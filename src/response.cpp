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

#include <QDateTime>
#include <QIODevice>

#include "onion.h"
#include "response.h"

using namespace Onion;

/**
 * @short Initiailzes a header response
 */
Response::Response(){
	status=200;
	length=0xFFFFFFF;

	headers["server"]="Onionaemon v.0.0.1. test";
	headers["date"]=QDateTime::currentDateTime().toString(Qt::ISODate);
	//headers["Connection"]="Close";
	//headers["Keep-Alive"]="15";
	ignoreHeaders=false;
}


/**
 * @short Returns the header as a QString, prepared to be sent
 */
QByteArray Response::headerAsByteArray(){
	QString r=QString("HTTP/1.0 %1 %2\n").arg(status).arg("OK");
	QString value;
	QByteArray ba;

	foreach(QString key,headers.keys()){
		value=headers[key];
		if (!value.isEmpty())
			r+=key+": "+value+"\n";
	}
	r+="\n";

	ba=r.toUtf8();

	return ba;
}


/**
 * @short Sets the header. For some special headers do something.
 */
void Response::setHeader(const QString &key, const QString &value){
	if (value.isNull()){
		headers.remove(key);
		return;
	}
	if (key=="content-length")
		length=value.toInt();
	headers[key.toLower()]=value;
}

/**
 * @short Shortcut to set the header about the length
 */
void Response::setLength(quint64 l){ 	
	headers["content-length"]=QString::number(l);
	length=l; 
}

/**
 * @short Shortcut to express the desire (or not) to keepalive
 */
void Response::setKeepAlive(bool activate){
	if (activate){
		setHeader("keep-alive","timeout=15, max=98");
		setHeader("connection","keep-alive");
	}
	else{
		setHeader("keep-alive",QString::null);
		setHeader("connection",QString::null);
	}
}
