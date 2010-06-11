/**
		onion+ - web small modular server.
		Copyright (C) 2007-2010 David Moreno Montero

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU Affero General Public License as
		published by the Free Software Foundation, either version 3 of the
		License, or (at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU Affero General Public License for more details.

		You should have received a copy of the GNU Affero General Public License
		along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDateTime>
#include <QDir>

#include "debug.h"
#include "client.h"
#include "response.h"
#include "module.h"

using namespace Onion;

Client::Client(QTcpSocket *client, Module *root) : request(client), root(root){
	atHeader=2;
	fromModule=NULL;

	connect(client,SIGNAL(readyRead()), this, SLOT(readRequest()));
	connect(client,SIGNAL(bytesWritten(qint64)),
					this,SLOT(readAndWrite()));

	DEBUG("Client %p",this);
	readRequest();
}


Client::~Client(){
	// connected deleteLater to disconnect at daemon.cpp.
	DEBUG("~Client %p",this);
	delete fromModule;
}

/**
 * @short reads request from the client
 *
 * Reads all bytes needed for the request, and sets the request handler.
 */
void Client::readRequest(){
	qint64 len;
	char line[1024];

	QTcpSocket *client=dynamic_cast<QTcpSocket*>(request.client());
	while(client->canReadLine()){
		len=client->readLine(line,sizeof(line)/sizeof(line[0]));
		QString s;
		foreach(QString ss,QString(line).split('\n')){
			s=ss.simplified();
			if (atHeader){
				if (s=="") // Always looking for header end...
					atHeader--;
				else
					atHeader=2;

				request.addHeader(s);
			}
			if (!atHeader){
				processPetition();
				return;
			}
		}
	}
}

#define QS(str) ((char*)str.toUtf8().data())


/**
 * @short Calls the ProcessorFatory to perform the real calls.
 */
void Client::processPetition(){
	response=Response();
	bytesSent=0;

	DEBUG("GET %s",request.getGET().toAscii().data());
	
	if (root){
		if (fromModule)
			delete fromModule;
		fromModule=root->process(request,response);
	}

	//qDebug("%s:%d processing by %s",__FILE__,__LINE__,fromModule ? fromModule->metaObject()->className() : "NULL");
	if (!fromModule){
		response.setStatus(500);
		request.client()->write(response.headerAsByteArray());
		ERROR("Nobody could response your petition!");
		request.client()->write("<h1>404 - not found</h1>");
		request.client()->close();
		return;
	}
	else{
		qDebug("%s %s %s (to %s (%s) from %s, %d bytes)",
			QS(QDateTime::currentDateTime().toString(Qt::ISODate)),
			QS(request.getMethod()),
			QS(request.getFullPath()),
			QS(request.get("Host")),
			QS(request.get("Host-IP")),
			QS(request.client()->peerAddress().toString()),
			response.getHeader("Content-Length").toInt());
	}

	request.client()->write(response.headerAsByteArray());

	readAndWrite();
}

/**
 * @short Takes a little bit of data from input, and writes to output.
 *
 * Doing it here allows me to be multiconnection on single threads.
 */
void Client::readAndWrite(){
	if (!fromModule)
		return;
	if (response.getLength()<=bytesSent){
		fromModule->close();
	}

	if (fromModule->atEnd() || !fromModule->isOpen()){ // Do nothing. Hangs this connection; maybe more petitions later.
		// closes if any of these conditions
		if (response.getHeader("content-length").isEmpty() ||        // unknown length
				response.getHeader("connection").toLower()=="close" ||   // i forced close
				request.get("connection").toLower()!="keep-alive" ||     // other side does not want keep alive
				response.getHeader("keep-alive").isEmpty()) {            // i dont want keep alive
			DEBUG("Close %p",this);
			request.client()->disconnect();
			deleteLater();
		}
		else{ // Ready to get another...
			DEBUG("Keep alive %p",this);
			atHeader=2;
			request.clean();
		}
		return;
	}

	while(request.client()->bytesToWrite()<=0){
		QByteArray output;
		output=fromModule->read(2*1024);
		bytesSent+=request.client()->write(output); // buffer size should be tweakable.
	}
}
