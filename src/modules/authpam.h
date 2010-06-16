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

#ifndef AUTHPAM_H
#define AUTHPAM_H

#include "../module.h"

namespace Onion{
	/**
	 * @short Allows access to some area based on PAM authentication.
	 */
	class AuthPAM : public Module
	{
		public:
			AuthPAM(const QString &realm,const QString &pamname);
		
			QIODevice *process(Request &req, Response &res);
		protected:
			bool authorize(const QString &username, const QString &password);
			
			QString realm;
			QString pamname;
	};
}

#endif // AUTHPAM_H
