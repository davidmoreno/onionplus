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

#include "../modulelist.h"
#include "../configparser.h"

using namespace Onion;

Module *includeConstructor(const QStringList &l,const QMap<QString,QString> &){
  ModuleList *list=new ModuleList();
  Module *mod;
  bool error;
  
  foreach(QString file, l){
    //qDebug("%s:%d import %s",__FILE__,__LINE__,(char*)file.toUtf8().data());
    ConfigParser c(file);
    mod=c.getRoot();

    if (mod)
      list->append(mod);
    else
      error=true;
  }

  if (error){
    delete list;
    return NULL;
  }
  else
    return list;
}
