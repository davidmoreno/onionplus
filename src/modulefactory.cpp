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

#include "debug.h"
#include "modulefactory.h"
#include "module.h"

using namespace Onion;

ModuleFactory factory;

Module *pathConstructor(const QStringList &,const QMap<QString,QString> &);
Module *serverConstructor(const QStringList &,const QMap<QString,QString> &);
Module *includeConstructor(const QStringList &,const QMap<QString,QString> &);
Module *fakeConstructor(const QStringList &,const QMap<QString,QString> &);
Module *staticFileConstructor(const QStringList &,const QMap<QString,QString> &);
Module *jsonDirListConstructor(const QStringList &,const QMap<QString,QString> &);
Module *directoryConstructor(const QStringList &,const QMap<QString,QString> &);
Module *activateModule(const QStringList &,const QMap<QString,QString> &);
Module *statsConstructor(const QStringList &,const QMap<QString,QString> &);

/**
 * @short Register basic modules
 */
ModuleFactory::ModuleFactory(){
	struct DefaultModulesStruct{
		const char *name;
		ModuleConstructor constructor;
	};
	struct DefaultModulesStruct defaultModules[] = {
		{"fake",&fakeConstructor},
		{"Include",&includeConstructor},
		{"[[",&serverConstructor},
		{"[",&pathConstructor},
		{"StaticFile",&staticFileConstructor},
		{"Directory",&directoryConstructor},
		{"ActivateModule",&activateModule},
		{"Stats",&statsConstructor},
	};

	for (unsigned int i=0;i<sizeof(defaultModules)/sizeof(defaultModules[0]);i++){
		ModuleFactory::registerModule(defaultModules[i].name,defaultModules[i].constructor);
	}
	//ModuleFactory::registerModule("JsonDirList",&jsonDirListConstructor);
}

/**
 * @short From the given parameters, try to match a moduleConstructor and construct
 */
Module *ModuleFactory::getModule(
										  const QString &moduleName, 
										  const QStringList &positionalParameters,
										  const QMap<QString,QString> &namedParams){
	if (!factory.moduleConstructors.contains(moduleName)){
		return NULL;
	}
	ModuleConstructor mc=factory.moduleConstructors[moduleName];

	return mc(positionalParameters, namedParams);
}

/**
 * @short Registers a new moduleConstructor from the given name
 *
 * It returns whether there was already a module with that name (ie. If it
 * replaced an older version).
 */
bool ModuleFactory::registerModule(const QString &name, ModuleConstructor mc){
	LOG("register module %s",name.toUtf8().constData());
	bool replace=factory.moduleConstructors.contains(name);
	factory.moduleConstructors[name]=mc;
	return replace;
}
