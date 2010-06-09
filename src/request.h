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

#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <QMap>
#include <QStringList>
#include <QString>

class QTcpSocket;

/**
 * @short Data from the request.
 */
class Request {
public:
	Request(QTcpSocket *client);
	~Request();

	void addHeader(const QString &header);

	void clean();

	QString getPath() const{ return path; }
	void    setPath(const QString &_path){ path=_path; }
	QString getFullPath() const{ return fullpath; }
	QString getMethod() const{ return method; }
	QString get(const QString &key) const{ return headers[key]; }
	bool isValid() const{ return valid; }
	QTcpSocket *client(){ return m_client; }
protected:
	QTcpSocket *m_client;
	QMap<QString,QString> headers;
	QString path;
	QString fullpath;
	QString method;

	bool valid;
};

#endif
