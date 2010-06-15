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

#include <QFile>
#include <QFileInfo>

#include "resources.h"

using namespace Onion;

Module *resourcesConstructor(const QStringList &l,const QMap<QString,QString> &m){
	if (l.count()!=0 || m.count()>0)
		return NULL;
	
	return new Resources();
}

/**
 * @short Tries to open a resource by name, and if availabe, just returns it.
 */
QIODevice *Resources::process(Request &req, Response &res){
	QString resourceName=QString(":") + req.getPath();
	
	if (!QFile::exists(resourceName)){
		return NULL;
	}
	QFile *ret=new QFile(resourceName);
	ret->open(QIODevice::ReadOnly);
	return ret;
}
