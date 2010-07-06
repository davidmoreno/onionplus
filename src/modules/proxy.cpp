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

#include <QTcpSocket>

#include "proxy.h"
#include "debug.h"
#include "file.h"

using namespace Onion;


Module *proxyConstructor(const QStringList &l,const QMap<QString,QString> &m){
	if (l.count()!=1 || m.count()>0)
		return NULL;
	

	Proxy *d=new Proxy(l[0]);
	//qDebug("%s:%d New directory: %s %p",__FILE__,__LINE__,QS(l[0]),d);

	return d;
}


Proxy::Proxy(const QString &server): Module("Proxy"), server(server){
	port=80;
	if (server.contains(":")){
		port=server.section(":",1,1).toInt();
		this->server=server.section(":",0,0);
	}
	DEBUG("Created proxy to %s : %d",this->server.toAscii().constData(), port);
}

QIODevice *Proxy::process(Request &request, Response &response){
	DEBUG("Connecting to proxy %s : %d",this->server.toAscii().constData(), port);
	QTcpSocket *socket=new QTcpSocket;
	socket->connectToHost(server, port);

	if (!socket->waitForConnected()) {
		DEBUG("Not connected");
		return new File(":502.html", request, response);
	}
	QMap<QString,QString> headers=request.getHeaders();
	QMap<QString,QString>::iterator I=headers.begin(), endI=headers.end();
	DEBUG("GET: %s",QString("%1 /%2").arg(request.getMethod()).arg(request.getPath()).toUtf8().data());
	socket->write(QString("%1 /%2 HTTP/1.1\r\n").arg(request.getMethod()).arg(request.getPath()).toUtf8());
	while (I!=endI){
		DEBUG("Header %s: %s", I.key().toAscii().data(),I.value().toAscii().data() );
		socket->write(QString("%1: %2\r\n").arg(I.key()).arg(I.value()).toUtf8());
		++I;
	}
	socket->write("\r\n");
	socket->flush();
	response.setIgnoreHeaders(true);
	
	DEBUG("Done");
	return socket;
}
