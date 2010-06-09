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

#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>
#include <QStringList>
#include <QRegExp>

#include "configparser.h"
#include "daemon.h"
#include "onion.h"


int main(int argc, char **argv){
  QCoreApplication *app=new QCoreApplication(argc, argv);
	QString configfile;
	
	if (app->arguments().length()==2)
		configfile=app->arguments().at(1);
	else
		configfile="/etc/onion+.conf";
	
  Onion::Daemon d(configfile);

  app->exec();
}



/**
 * @short Decodes an URL to a real filename: "Hola%20mundo" -> "Hola mundo".
 *
 * Uses a conversion table. It is slow.
 */
QString decodeURL(const QString &orig){
  //qDebug("%s:%d try decode %s",__FILE__,__LINE__,QS(orig));
  QString dest=orig;
  QString capt;
  int pos=0;
  {
    QRegExp rx("%([a-fA-F0-9][a-fA-F0-9])%([a-fA-F0-9][a-fA-F0-9])");
    while ((pos = rx.indexIn(orig, pos)) != -1) {
      char letter[3]={rx.cap(1).toInt(NULL,16),rx.cap(2).toInt(NULL,16),0};
      capt=QString::fromUtf8(letter,2);
      dest=dest.replace(rx.cap(0),capt);

      pos += rx.matchedLength();
    }
  }
  {
    QRegExp rx("%([a-fA-F0-9][a-fA-F0-9])");
    pos=0;
    while ((pos = rx.indexIn(orig, pos)) != -1) {
      char letter[2]={rx.cap(1).toInt(NULL,16),0};
      capt=QString::fromLatin1(letter,1);
      dest=dest.replace(rx.cap(0),capt);

      pos += rx.matchedLength();
    }
  }


  //qDebug("%s:%d res-> %s",__FILE__,__LINE__,QS(dest));
  return dest;
}


/// Encodes an URL from a real filename: "Hola mundo" -> "Hola%20mundo".
QString encodeURL(const QString &str){
  QString ret=str;
  ret=ret.replace("%","%25");
  ret=ret.replace(" ","%20");
  return ret;
}

