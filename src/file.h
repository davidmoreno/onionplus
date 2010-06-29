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

#ifndef FILE_H
#define FILE_H

#include <QFile>

#include "request.h"
#include "response.h"

namespace Onion{
	/**
	 * @short Reader of a simple file from the system.
	 *
	 * It takes care of setting the size, mime type, and use cache if necesary (not yet).
	 */
	class File : public QFile
	{
		public:
			File(const QString &filename, Request &req, Response &res);
	};
}

#endif // FILE_H
