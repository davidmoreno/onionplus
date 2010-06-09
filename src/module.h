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

#ifndef __MODULE_H__
#define __MODULE_H__

class QIODevice;

#include <QtPlugin>
#include <QString>

#include "request.h"
#include "response.h"

namespace Onion{
  class Module;
  /**
   * @short Basic processor. Should return a QIODevice to return data to the client.
   *
   * This objects are constructed by the ConfigParser with positionalParams and 
   * namedParams.
   * The construction is performed by 'C' a function that is registered at 
   * ModuleFactory.
   *
   * Then they can be called at any time with a Request and a Response 
   * (process(...)); if processes correctly then must return a non-NULL 
   * QIODevice. They may (and actually should if everything goes correctly) call
   * nextModule.
   *
   * @see ModuleFactory::register(const QString &, moduleConstructor *);
   */
  class Module{
  public:
    virtual ~Module(){};

    /**
     * @short Must perform the real processing.
     *
     * Actually this method may do one of this three things:
     *
     *  a) Return NULL, that means that this module was not the appropiate
     *     for this Request.
     *  b) Pass the petition to the nextModule, and return its process(...).
     *  c) Return a QIODevice itself.
     *
     * In all three cases, headers may be modified: new env. vars at request,
     * new response headers at response...
     */
    virtual QIODevice *process(Request &, Response &){ return NULL; };

    /// Sets the next module in this chain.
    void setNextModule(Module *next){ nextModule=next; }
  protected:
    Module *nextModule;
  };

};

#endif
