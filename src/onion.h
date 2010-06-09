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

#ifndef __ONION_H__
#define __ONION_H__

#define QS(qs) ((char*)qs.toUtf8().data())

class QString;

/// Decodes an URL to a real filename: "Hola%20mundo" -> "Hola mundo".
QString decodeURL(const QString &);
/// Encodes an URL from a real filename: "Hola mundo" -> "Hola%20mundo".
QString encodeURL(const QString &);

#endif
