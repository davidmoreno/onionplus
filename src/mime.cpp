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
#include <QTextStream>
#include <QStringList>

#include "debug.h"
#include "mime.h"
#include <QFileInfo>

using namespace Onion;

Mime mimeSingleton;

/**
 * @short Loads the mime data from /etc/mime.types
 */
Mime::Mime(){
	DEBUG("Reading mime types");
	QFile f("/etc/mime.types");
	f.open(QIODevice::ReadOnly);
	QTextStream stream(&f);
	while (!f.atEnd()){
		QString l=f.readLine();
		if ( l.isEmpty() || l.startsWith("#") )
			continue;
		QStringList ll=l.simplified().split(" ");
		QString mime=ll[0];
		ll.pop_front();
		foreach(const QString ext, ll){
			//DEBUG("Adding %s extension, mimetype %s",ext.toAscii().constData(), mime.toAscii().constData());
			mimeMap[ext.toLower()]=mime;
		}
	}
	DEBUG("Added %d mime types",mimeMap.count());
}

static QString octetStream("application/octet-stream");

/**
 * @short Given a filename, returns the proper mime type (even if tis the unknown type)
 */
QString Mime::forFilename(const QString &name){
	//DEBUG("Mime types %s",QStringList(mimeSingleton.mimeMap.keys()).join(", ").toAscii().constData());
	QString ext=name.section(".",1,-1).toLower();
	QString ret=mimeSingleton.mimeMap[ext];
	//DEBUG("Look for extension for %s, got mime %s",ext.toAscii().constData(), ret.toAscii().constData());
	if (ret.isEmpty())
		return octetStream;
	return ret;
}
