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
#include <QDir>
#include <QBuffer>

#include "debug.h"
#include "onion.h"
#include "directory.h"

using namespace Onion;

Module *directoryConstructor(const QStringList &l,const QMap<QString,QString> &m){
	if (l.count()!=1 || m.count()>0)
		return NULL;
	

	Directory *d=new Directory(l[0]);
	//qDebug("%s:%d New directory: %s %p",__FILE__,__LINE__,QS(l[0]),d);

	return d;
}

/**
 * @short If the constructing RegExp matches, then return next in chain.
 */
QIODevice *Directory::process(Request &req,Response &res){
	if (!QDir::cleanPath(dirname+"/"+req.getPath()).contains(dirname)){
		ERROR("Trying to escape allowed path");
		return NULL;
	}

	QFile *f=new QFile(dirname+"/"+req.getPath());
	f->open(QIODevice::ReadOnly);
	if (!f->isOpen()){
		WARNING("File not found: %s %p",(char*)f->fileName().toLatin1().data(),this);
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

	res.setHeader("Content-Length",QString::number((long int)f->size()));

	return f;
}
