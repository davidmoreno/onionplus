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

#include "modulelist.h"

using namespace Onion;

void ModuleList::append(Module *mod){
  list.append(mod);
}

/**
 * @short Try all module chains, or return NULL
 */
QIODevice *ModuleList::process(Request &req, Response &res){
  QIODevice *ret;
  foreach(Module *mod, list){
    ret=mod->process(req,res);
    if (ret){
      return ret;
    }
  }
  qDebug("%s:%d could not use any",__FILE__,__LINE__);
  return NULL;
};
