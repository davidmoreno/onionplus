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

#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <QObject>
#include <QtNetwork/QTcpSocket>

#include "request.h"
#include "response.h"

class QTcpSocket;
class QIODevice;

namespace Onion{
	class Module;

	/**
	 * @short Keeps and coordinate petitions from clients.
	 * 
	 * 
	 */
	class Client : public QObject{
	Q_OBJECT
	public:
		Client(QTcpSocket *client, Module *root);
		~Client();
	
	protected slots:
		void readRequest();
		void processPetition();
		void readAndWrite();

	protected:
		int atHeader;
		Request request;
		Response response;
		unsigned long bytesSent;

		Module *root;
		QIODevice *fromModule;
	};
}
	
#endif
