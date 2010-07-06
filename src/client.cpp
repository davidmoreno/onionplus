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
#include "daemon.h"
#include "module.h"
#include "file.h"

using namespace Onion;

Client::Client(QTcpSocket *clientSocket, Daemon *daemon) : 
											request(this), 
											daemon(daemon), root(daemon->getRootModule()), clientSocket(clientSocket){
	request.addHeader("host-ip",clientSocket->peerAddress().toString());

	atHeader=2;
	fromModule=NULL;

	connect(clientSocket,SIGNAL(readyRead()), this, SLOT(readRequest()));
	connect(clientSocket,SIGNAL(bytesWritten(qint64)),
					this,SLOT(readAndWrite()));

	DEBUG("Client %p",this);
	readRequest();
}


Client::~Client(){
	// connected deleteLater to disconnect at daemon.cpp.
	daemon->decreaseActiveClientCount();
	DEBUG("~Client %p",this);
	delete fromModule;
	delete clientSocket;
}

/**
 * @short reads request from the client
 *
 * Reads all bytes needed for the request, and sets the request handler.
 */
void Client::readRequest(){
	qint64 len;
	char line[1024];

	while(clientSocket->canReadLine()){
		len=clientSocket->readLine(line,sizeof(line)/sizeof(line[0]));
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

	if (!fromModule){
		response.setStatus(500);
		fromModule=new File(":404.html", request, response);
	}
	DEBUG("Returning data with a %s",fromModule ? fromModule->metaObject()->className() : "NULL");
	
	
	qDebug("%s %s %s %d (to %s from %s, %d bytes)",
		QS(QDateTime::currentDateTime().toString(Qt::ISODate)),
		QS(request.getMethod()),
		QS(request.getFullPath()),
		response.getStatus(),
		QS(request.get("host")),
		QS(clientSocket->peerAddress().toString()),
		response.getHeader("Content-Length").toInt());
	
	if (!response.getIgnoreHeaders())
		clientSocket->write(response.headerAsByteArray());

	connect(fromModule,SIGNAL(readyRead()),
					this,SLOT(readAndWrite()));
	connect(fromModule,SIGNAL(disconnected()),
					this,SLOT(deleteLater()));

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
		DEBUG("Closed as response.getLength()<=bytesSent");
		fromModule->close();
	}

	// Do nothing. Hangs this connection; maybe more petitions later.
	DEBUG("Close? %d %d %d",response.getIgnoreHeaders() , fromModule->atEnd() , fromModule->isOpen());
	if ( (!response.getIgnoreHeaders() && fromModule->atEnd()) || !fromModule->isOpen() ){ 
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
			request.addHeader("Host-IP",clientSocket->peerAddress().toString());
		}
		return;
	}
	if (!(fromModule->bytesAvailable()>0)){ // not ready both, wait for next ready
		DEBUG("Not transmiting, bytesAvailable %d, bytesToWrite %d",(int)fromModule->bytesAvailable(),(int)clientSocket->bytesToWrite());
		return;
	}

	while(clientSocket->bytesToWrite()<=0){
		QByteArray output;
		if (!(fromModule->bytesAvailable()>0)){ // not ready both, wait for next ready
			DEBUG("Not transmiting, bytesAvailable %d, bytesToWrite %d",(int)fromModule->bytesAvailable(),(int)clientSocket->bytesToWrite());
			readAndWrite();
			return;
		}
		output=fromModule->read(2*1024);
		bytesSent+=clientSocket->write(output); // buffer size should be tweakable.
		DEBUG("Wrote %ld",bytesSent);
	}
}
