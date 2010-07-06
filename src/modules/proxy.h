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

#ifndef PROXY_H
#define PROXY_H
#include <module.h>

namespace Onion{

	/**
	 * @short Do as a reverse proxy, it redirects petitions to another server
	 */
	class Proxy : public Module
	{
		public:
			Proxy(const QString &server);
			
			QIODevice *process(Request &request, Response &response);
		protected:
			QString server;
			int port;
	};

}

#endif // PROXY_H
