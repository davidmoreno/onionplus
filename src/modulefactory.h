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

#ifndef __MODULE_FACTORY_H__
#define __MODULE_FACTORY_H__

#include <QString>
#include <QStringList>
#include <QMap>

#include "module.h"


#ifndef ONION_SERVER
# define ONION_MODULE(name, constructorfunction) extern "C" { void createOnionModule(){ ModuleFactory::registerModule(name,constructorfunction); } }
#else
# define ONION_MODULE(name, constructorfunction)
#endif


namespace Onion{

  /**
   * @short This type is the one used to construct modules.
   */
  typedef Module *(*ModuleConstructor)(const QStringList &positionalParameters, 
                                      const QMap<QString,QString> &namedParams);
  

  /**
   * @short Factory that creates the real modules.
   */
  class ModuleFactory{
  public:
    ModuleFactory();

    static Module *getModule(const QString &moduleName, 
                      const QStringList &positionalParameters,
                      const QMap<QString,QString> &namedParams);

    static bool registerModule(const QString &name,ModuleConstructor );

  protected:
    QMap<QString,ModuleConstructor> moduleConstructors;
  };
};


#define DAVD_REGISTER_MODULE(name,constructor) void init{ Onion::ModuleFactory::registerModule(name,constructor); }




#endif
