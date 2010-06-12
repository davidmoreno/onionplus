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
#include <QBuffer>

#include "staticfile.h"

using namespace Onion;

Module *staticFileConstructor(const QStringList &l,const QMap<QString,QString> &m){
	int errorCode=200;

	if (l.count()!=1 || m.count()>1)
		return NULL;
	if (m.count()==1){
		QString code=m["code"];
		if (code.isEmpty())
			return NULL;
		bool ok;
		errorCode=code.toInt(&ok);
		if (!ok)
			return NULL;
	}

	return new StaticFile(l[0],errorCode);
}

/**
 * @short If the constructing RegExp matches, then return next in chain.
 */
QIODevice *StaticFile::process(Request &req,Response &res){
	DEBUG("Open file %s",(char*)filename.toLatin1().data());
	QFile *f=new QFile(filename);
	f->open(QIODevice::ReadOnly);
	res.setStatus(errorCode);

	if (!f->isOpen()){
		f->setFileName(":error.html");
		f->open(QIODevice::ReadOnly);
		res.setStatus(404);
	}
	
	res.setHeader("Content-Length",QString::number(f->size()));
	WARNING("Fixed content type. FIXME");
	res.setHeader("Content-Type","text/html; charset=utf-8");

	return f;
}
