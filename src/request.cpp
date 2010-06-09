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

#include "onion.h"
#include "request.h"

/**
 * @short Decompose headers to extract data and ease access to headers
 */
Request::Request(QTcpSocket *client):m_client(client){
	headers["Host-IP"]=client->peerAddress().toString();
	valid=true;
}

Request::~Request(){
	delete m_client;
}

/**
 * @short Adds a header from a formated string: 'key: value'.
 *
 * On the first call it sets the method, only GET BTM.
 */
void Request::addHeader(const QString &header){
	//qDebug("%s:%d addheader %s (%d)",__FILE__,__LINE__,(char*)header.toUtf8().data(),headers.count());
	if (method.isEmpty()){
		QStringList spl;
		spl=header.split(' ');
		method=spl[0].toUpper();
		path=decodeURL(spl[1]);
		if (path.contains("?")){
			path=path.split("?")[0];
		}
		fullpath=path;
		if (method!="GET"){
			valid=false;
		}
		return;
	}


	QString key=header.section(':',0,0).simplified().toLower();
	QString value=header.section(':',1,-1).simplified();
	if (!value.isEmpty() && key!="Host-IP"){ // Unrewritable Host-IP
		headers[key]=value;
		//qDebug("%s:%d new header '%s':'%s'",__FILE__,__LINE__,key.toLatin1().data(),value.toLatin1().data());
	}
}

/**
 * @short Cleans the petition
 */
void Request::clean(){
	method=QString::null;
	headers.clear();
	valid=true;
}
