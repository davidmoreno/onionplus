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

#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__

#include <QString>
#include <QMap>

class QIODevice;

namespace Onion{
	class Module;

	/**
	 * @short Parses a config file (or QIODevice) and returns the root module
	 */
	class ConfigParser{
	public:
		ConfigParser(const QString &filename=QString::null);
		
		Module *getRoot();
		bool getHasErrors(){ return hasErrors; }
	protected:
		enum Token{
			STRING=129,
			SERVER=130,
			PATH=131,
			IDENTIFIER=132,
			END_OF_FILE=254,
			UNKNOWN=255
		};

		Token readToken(QString &token);
		QString readString(char delimiter);
		QString readIdentifier(char firstChar);
		bool skipComment();

		QString readParameters(QStringList &positional, QMap<QString,QString> &named);

		Module *getModule();
		Module *getNextModule();
		Module *getModuleList();
		Module *getServerOrPath();

		void printError(const QString &);

	protected:
		QIODevice *input;
		Module *root;
		bool hasErrors;
		QString filename;

		Token t;
		QString r;
	

		/// To ease config debug.
		int line, col;
	};
};


#endif
