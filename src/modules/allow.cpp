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

#include "allow.h"

using namespace Onion;


Module *allowConstructor(const QStringList &l,const QMap<QString,QString> &m){
	if (l.count()!=1 || m.count()>0)
		return NULL;
	
	return new Allow(l);
}


Allow::Allow(QStringList IPranges) : Module("Allow"), IPranges(IPranges){
}


QIODevice *Allow::process(Request &req, Response &res){
	DEBUG("%s in %s",IPranges.join("|").toAscii().constData(), req.getHeader("Host-IP").toAscii().constData());
	if (IPranges.indexOf(req.getHeader("Host-IP"))>=0)
		return nextModule ? nextModule->process(req,res) : NULL; 
	return NULL;
}
