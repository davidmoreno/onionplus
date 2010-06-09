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
	qDebug("%s:%d Open file %s",__FILE__,__LINE__,(char*)filename.toLatin1().data());
	QFile *f=new QFile(filename);
	f->open(QIODevice::ReadOnly);
	if (!f->isOpen()){
		QBuffer *r=new QBuffer();
		r->open(QBuffer::ReadWrite);
		r->write("<h1>404 - File '");
		r->write((char*)f->fileName().toLatin1().data());
		r->write("' not fould. Check server configuration.<h1>");
		res.setHeader("Content-Length",QString::number(r->pos()));
		r->seek(0);
		res.setHeader("Keep-Alive","timeout=15, max=98");
		res.setHeader("Connection","Keep-Alive");
		delete f;
		
		return r;
	}
	
	res.setHeader("Content-Length",QString::number(f->size()));
	res.setHeader("Content-Type","text/html; charset=utf-8");
	res.setStatus(errorCode);

	return f;
}
