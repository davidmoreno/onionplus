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

#include "../modulefactory.h"
#include "jsondirlist.h"

using namespace Onion;

Module *jsonDirListConstructor(const QStringList &l,const QMap<QString,QString> &m){
  if (l.count()!=1 || m.count()>0)
    return NULL;

  return new JsonDirList(l[0]);
}

ONION_MODULE("JsonDirList",jsonDirListConstructor);

QString sanitizeForJson(const QString &str){
  QString s=str;
  s=s.replace("'","\\'");
  return s;
}


#define QS(str) ((char*)str.toUtf8().data())

/**
 * @short Returns s Json list with the asked directory contents.
 */
QIODevice *JsonDirList::process(Request &req,Response &res){
  QDir d(basedir+req.getPath());
  qDebug("%s:%d ask for %s",__FILE__,__LINE__,QS(d.absolutePath()));
  if (!(d.absolutePath()+"/").contains(basedir)){
    qDebug("%s:%d trying to escape allowed path! (try: %s, allowed: %s)",
      __FILE__,__LINE__,QS(d.absolutePath()),QS(basedir));
    return NULL;
  }
  QFileInfoList l=d.entryInfoList();

  QStringList fl;
  foreach(QFileInfo f, l){
    QString type;
    if (f.isDir())
      type="directory";
    else
      type=f.suffix();

    fl.append(QString("  {'name':'%1','size':%2,'date':'%3','type':'%4'}").
                arg(sanitizeForJson(f.fileName())).arg(f.size()).
                arg(f.created().toString(Qt::ISODate)).arg(type));
  }

  QBuffer *r=new QBuffer();
  r->open(QBuffer::ReadWrite);

  QString dl=QString("{'files':[%1]}\n").arg(fl.join(",\n"));
  r->write(dl.toUtf8());
  r->seek(0);
  res.setHeader("Content-Length",QString::number(dl.count()));
  res.setHeader("Keep-Alive","timeout=15, max=98");
  res.setHeader("Connection","Keep-Alive");

  return r;
}
