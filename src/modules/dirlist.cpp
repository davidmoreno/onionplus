/**
		onion+ - web small modular server.
		Copyright (C) 2007-2008 David Moreno Montero

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

#include <QBuffer>
#include <QDir>
#include <QDateTime>
#include <QUrl>

#include "../modulefactory.h"
#include "dirlist.h"
#include "../onion.h"
#include <mime.h>

using namespace Onion;

Module *dirListConstructor(const QStringList &l,const QMap<QString,QString> &m){
	if (l.count()!=1 || m.count()>0)
		return NULL;

	return new DirList(l[0]);
}

#ifdef ONION_PLUGIN
ONION_MODULE("DirList",dirListConstructor);
#endif

/**
 * @short Initializes the dirlist. Loads necesary data.
 */
DirList::DirList(const QString &s):Module("DirList"),basedir(s){ 
};
 

/**
 * @short Returns s Json list with the asked directory contents.
 */
QIODevice *DirList::process(Request &req,Response &res){
	DEBUG("%s",QS(req.getPath()));
	DEBUG("%s",QS(req.getQuery()));

	QDir d(basedir+req.getPath());
	if (!(d.absolutePath()+"/").contains(basedir)){
		ERROR("Trying to escape from allowed path! (try: %s, allowed: %s)",
						QS(d.absolutePath()),QS(basedir));
		return NULL;
	}
	if (!d.exists()){
		// Try a file
		if (QFile::exists(basedir+req.getPath())){
			QFile *f=new QFile(basedir+req.getPath());
			f->open(QIODevice::ReadOnly);
			res.setLength(f->size());
			res.setKeepAlive(true);
			return f;
		}
		return NULL;
	}
	
	QFileInfoList l=d.entryInfoList();

	QBuffer *r=new QBuffer();
	r->open(QBuffer::ReadWrite);

	r->write("<html>");
	r->write(QString("<title>%1</title>\n").arg(req.getPath()).toUtf8());
	r->write("<style>@import url(\"/resources/onion.css\");</style>\n");
	r->write(
"<body>\n"
"<img src=\"/resources/onion.png\">\n"
"<div><h1>/");
	r->write(req.getPath().toAscii());
	r->write("</h1><table><tr class=\"header\"><th>Filename</th><th>Size</th><th>Date</th><th>Type</th></tr>\n");

	QStringList fl;
	QString filename;
	foreach(QFileInfo f, l){
		QString type;
		if (f.isDir()){
			type="<em>directory</em>";
			filename=f.fileName()+"/";
		}
		else{
			filename=f.fileName();
			type=Mime::forFilename(filename);
			//type=QString("<b>%1</b>/%2").arg(type.section("/",0,0)).arg(type.section("/",1,1));
			if (type=="application/octet-stream")
				type="<em>application/octet-stream</em>";
		}
		r->write(QString("<tr onclick=\"location+='%6';\"><td><a href=\"%1\">%2</a></td><td class=\"size\">%3</td><td class=\"date\">%4</td><td>%5</td></tr>\n").
								arg(encodeURL(filename)).arg(filename).arg(f.size()).
								arg(f.created().toString("yyyy-MM-dd hh:mm")).arg(type).
								arg(encodeURL(filename))
								.toUtf8());
	}

	r->write("</table><b class=\"c\">(C) 2010 David Moreno Montero</b>\n");
	r->write("</div>\n</body>\n</html>\n");

	res.setLength(r->pos());
	res.setHeader("Content-Type","text/html; charset=UTF-8");
	res.setKeepAlive(true);
	r->seek(0);

	return r;
}
