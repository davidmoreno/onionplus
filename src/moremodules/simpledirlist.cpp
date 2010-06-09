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
#include "simpledirlist.h"
#include "../onion.h"

using namespace Onion;

Module *simpleDirListConstructor(const QStringList &l,const QMap<QString,QString> &m){
  if (l.count()!=1 || m.count()>0)
    return NULL;

  return new SimpleDirList(l[0]);
}

ONION_MODULE("SimpleDirList",simpleDirListConstructor);


/**
 * @short Returns s Json list with the asked directory contents.
 */
QIODevice *SimpleDirList::process(Request &req,Response &res){
  QDir d(basedir+req.getPath());
  //qDebug("%s:%d ask for %s",__FILE__,__LINE__,QS(d.absolutePath()));
  if (!(d.absolutePath()+"/").contains(basedir)){
    //qDebug("%s:%d trying to escape from allowed path! (try: %s, allowed: %s)",
    //  __FILE__,__LINE__,QS(d.absolutePath()),QS(basedir));
    return NULL;
  }
  QFileInfoList l=d.entryInfoList();

  QBuffer *r=new QBuffer();
  r->open(QBuffer::ReadWrite);

  r->write(
"<html><title>\n"
"<style type=\"text/css\">\n"
"table,td{\n"
"  border: 1px solid black;\n"
"  width: 100%;\n"
"  border-collapse: collapse;\n"
"}\n"
"a:hover {\n"
"  text-decoration: none;\n"
"  background-color: #DDD;\n"
"  display: block;\n"
"}\n"
"a {\n"
"  text-decoration: none;\n"
"  background-color: #FFF;\n"
"  display: block;\n"
"}\n"
"</style>\n"
"<body>\n"
"<img src=\"/img/onion.png\">\n"
"<table><tr><th>Filename</th><th>Size</th><th>Date</th><th>Type</th></tr>\n");

  QStringList fl;
  QString filename;
  foreach(QFileInfo f, l){
    QString type;
    if (f.isDir()){
      type="directory";
      filename=f.fileName()+"/";
    }
    else{
      type=f.suffix();
      filename=f.fileName();
    }
    r->write(QString("<tr><td><a href=\"%1\">%2</a></td><td>%3</td><td>%4</td><td>%5</td></tr>\n").
                arg(encodeURL(filename)).arg(filename).arg(f.size()).
                arg(f.created().toString(Qt::ISODate)).arg(type).toUtf8() ) ;
  }

  r->write("</table></body></html>");

  res.setHeader("Content-Type","text/html; charset=UTF-8");
  res.setHeader("Content-Length",QString::number(r->pos()));
  res.setHeader("Keep-Alive","timeout=15, max=98");
  res.setHeader("Connection","Keep-Alive");
  r->seek(0);

  return r;
}
