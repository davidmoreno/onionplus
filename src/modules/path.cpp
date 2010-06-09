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

#include "onion.h"
#include "path.h"

using namespace Onion;

Module *pathConstructor(const QStringList &l,const QMap<QString,QString> &m){
  if (l.count()!=1 || m.count()!=0)
    return NULL;
  if (l[0].endsWith("$"))
    return new Path(l[0]);
  else
    return new Path("^"+l[0]);
}

/**
 * @short If the constructing RegExp matches, then return next in chain.
 */
QIODevice *Path::process(Request &req,Response &res){
  QString checkPath=req.getPath();
  if ((path.indexIn(checkPath)>=0) && nextModule){
    req.setPath(checkPath.remove(path));
    //qDebug("%s:%d path %s in  %s",__FILE__,__LINE__,QS(checkPath),QS(path.pattern()));
    return nextModule->process(req,res);
  }
  //qDebug("%s:%d skip path %s in %s",__FILE__,__LINE__,QS(checkPath),QS(path.pattern()));

  return NULL;
}
