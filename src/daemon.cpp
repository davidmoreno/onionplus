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

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <resolv.h>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include "module.h"
#include "debug.h"
#include "configparser.h"
#include "client.h"
#include "daemon.h"

using namespace Onion;

/**
 * @short starts the server and connection.
 */
Daemon::Daemon(const QString &configfile): config(configfile) {
	root=config.getRoot();

	if (config.getHasErrors()){
		ERROR("Errors parsing config file. Stoping!");
		exit(1);
	}

	maxActiveClientCount=clientCount=activeClientCount=0;

	server=new QTcpServer(this);
	connect(server,SIGNAL(newConnection()), this, SLOT(newConnection()));

	server->listen(QHostAddress::Any, 8080);
	LOG("listening %s:%d",server->serverAddress().toString().toAscii().data(),
														server->serverPort());
}

/**
 * @short Do the real server serve: connect clients to real server object.
 */
void Daemon::newConnection(){
	
	QTcpSocket *socket=server->nextPendingConnection();
	while(socket){
		//qDebug("%s:%d newConnection",__FILE__,__LINE__);
		DEBUG("new connection");

		// Create client
		Client *client=new Client(socket, this);
		clientCount++;
		activeClientCount++;
		if (activeClientCount>maxActiveClientCount)
			maxActiveClientCount=activeClientCount;
		
		connect(socket,SIGNAL(disconnected()), client, SLOT(deleteLater()));
		//connect(socket,SIGNAL(disconnected()), socket, SLOT(deleteLater()));
		socket=server->nextPendingConnection();
	}
}

