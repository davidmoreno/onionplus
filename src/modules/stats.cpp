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

#define QS(qs) (qs.toUtf8().constData())

#include "daemon.h"
#include "client.h"
#include "stats.h"

using namespace Onion;

Module *statsConstructor(const QStringList &l,const QMap<QString,QString> &m){
	return new Stats(l[0]);
}

Stats::Stats(const QString &where) : Module("Stats"), where(where){
	requests=0;
}

QIODevice *Stats::process(Request &req,Response &res){
	requests++;
	DEBUG("<%s> = <%s>",QS(req.getPath()), QS(where));
	if (req.getPath()!=where)
		return nextModule ? nextModule->process(req,res) : NULL;
	else{
		QBuffer *r=new QBuffer();
		r->open(QBuffer::ReadWrite);
		r->write("<html>");
		r->write(QString("<title>%1</title>\n").arg(req.getPath()).toUtf8());
		r->write("<style>@import url(\"/?onion.css\");</style>\n");
		r->write(
"<body>\n"
"<img src=\"/?onion.png\">\n"
"<div><h1>Stats</h1>");
		r->write("<table>\n");
		
		r->write("<tr><th>Request account</th><td>");
		r->write(QString::number(requests).toUtf8());
		r->write("</td></tr>\n");
		
		r->write("<tr><th>Clients created</th><td>");
		r->write(QString::number(req.client()->getDaemon()->getClientCount() ).toUtf8());
		r->write("</td></tr>\n");

		r->write("<tr><th>Active client count</th><td>");
		r->write(QString::number(req.client()->getDaemon()->getActiveClientCount() ).toUtf8());
		r->write("</td></tr>\n");

		r->write("<tr><th>Maximum active client count</th><td>");
		r->write(QString::number(req.client()->getDaemon()->getMaxActiveClientCount() ).toUtf8());
		r->write("</td></tr>\n");
		
		r->write("</table>\n");

		r->write("<b>(C) 2010 David Moreno Montero</b>\n");
		r->write("</div>\n");
		r->write("</body></html>");
		res.setLength(r->pos());
		res.setHeader("Content-Type","text/html; charset=UTF-8");
		res.setKeepAlive(true);
		r->seek(0);

		return r;
	}
}


