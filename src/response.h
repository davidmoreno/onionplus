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

#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <QMap>
#include <QString>

#define HTTP_OK 200
#define HTTP_FOUND 302
#define HTTP_BAD_REQUEST 400
#define HTTP_UNAUTHORIZED 401
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404


namespace Onion{
	class Request;

	/**
	* @short The HTTP response. 
	*
	* Just sets/gets the response header. Processor does the job.
	*/
	class Response {
	public:
		Response();
		/**
		* @short Adds a header value
		*/
		void setHeader(const QString &key, const QString &value);
		/** 
		* Returns a header value from the key, or QString::null
		*/

		QString getHeader(const QString &key){ return headers[key.toLower()]; }
		void setStatus(int s){ status=s; }
		int getStatus(void){ return status; }
		unsigned long getLength(){ return length; }


		void setLength(quint64 l);
		void setKeepAlive(bool activate);

		/// Just writes the header
		QByteArray headerAsByteArray();

	protected:
		QMap<QString,QString> headers;

		quint64 length;
		int status;
	};
}


#endif
