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

#ifndef __DAEMON_H__
#define __DAEMON_H__

#include <QObject>

class QTcpServer;
class Module;

namespace Onion{
  /**
   * @short Base daemon. Gets the request and send them to the clients.
   */
  class Daemon : public QObject{
  Q_OBJECT
  public:
    Daemon(const QString &configfile);
  
  protected slots:
    void newConnection();
  
  protected:
    QTcpServer *server;
  
    Module *root;
  };
}
  
#endif
