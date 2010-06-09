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

#include <dlfcn.h>
#include <stdlib.h>

#include "path.h"

using namespace Onion;

Module *activateModule(const QStringList &l,const QMap<QString,QString> &m){
  foreach(QString module, l){
    qDebug("%s:%d load module %s",__FILE__,__LINE__,(const char *)module.toAscii().data());
    
    void *onionmodule;
    
		if (!QFile::exists(module)){
			QString tmodule=QString("%1/%2").arg(getenv("ONIONDIR")).arg(module);
			if (QFile::exists(tmodule))
				module=tmodule;
		}
		
    onionmodule=dlopen((const char *)module.toAscii().data(), RTLD_NOW|RTLD_GLOBAL);
    if (onionmodule){
      void (*createOnionModule)(void);
      createOnionModule=(void(*)(void))dlsym(onionmodule,"createOnionModule");
      if (!createOnionModule){
        qDebug("%s:%d cant load module %s! Aborting! (%s)",__FILE__,__LINE__,(const char *)module.toAscii().data(), dlerror());
        return NULL;
      }
      createOnionModule();
    }
    else{
      qDebug("%s:%d cant load module %s! Aborting! (%s)",__FILE__,__LINE__,(const char *)module.toAscii().data(), dlerror());
      return NULL;
    }
  }

  return new Module();
}

