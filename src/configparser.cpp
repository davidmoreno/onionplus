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

#include <QIODevice>
#include <QFile>

#include "configparser.h"
#include "modulefactory.h"
#include "module.h"
#include "modulelist.h"

using namespace Onion;


/**
 * @short If a filename is given then create the QIODevice
 *
 * Parsing is delayed until getRoot is called (it is actually cached)
 */
ConfigParser::ConfigParser(const QString &filename) : filename(filename){
  hasErrors=false;
  root=NULL;
  line=1; col=0;

  QFile *f=new QFile(filename);
  input=f;

  if (!f->open(QIODevice::ReadOnly)){
    printError(QString("Cant open file: %1").arg(f->errorString()));
  }


}


/**
 * @short Reads a basic token (a special char, var name, string...)
 */
ConfigParser::Token ConfigParser::readToken(QString &token){
  char c;

  // This while is to be able to easily (and non-recursively) skip comments, spaces, newlines, tabs...
  while (true){
    if (!input->getChar(&c)){
      if (input->atEnd())
	return END_OF_FILE;
      printError("Input/Output Error");
      return UNKNOWN;
    }
    col++;
    switch(c){
      case '"':
      case '\'':
	token=readString(c);
	return STRING;
      case '\n':
	col=0;
	line++;
	break;
      case ' ':
      case '\t':
	break;
      case '/':
	if (!skipComment())
	  return UNKNOWN;
	break;
      default:
	if ((c>='a' && c<='z') || (c>='A' && c<='Z') || (c=='_')){
	  token=readIdentifier(c);
	  return IDENTIFIER;
	}
	else{
	  token=QString::null;
	  return (Token)c;
	}
      }
  }
}

/**
 * @short gets a string until delimiter is found (without a scaping \).
 */
QString ConfigParser::readString(char delimiter){
  bool escape=FALSE;
  char c;

  QString str="";
  while(input->getChar(&c)){
    col++;
    if (!escape && c==delimiter)
      return str;
    if (c=='\n'){
      col=0;
      line++;
    }
    if (c=='\\' && !escape)
      escape=TRUE;
    else{
      escape=FALSE;
      str+=c;
    }
  }
  printError("Error reading string");
  return QString::null;
}

/**
 * @short Reads an identifier (modulename, parameter...) [a-zA-Z_][a-zA-Z0-9_]*
 */
QString ConfigParser::readIdentifier(char firstChar){
  QString id;
  id+=firstChar;
  char c;
  while(input->getChar(&c)){
    col++;
    if ((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || (c=='_'))
      id+=c;
    else{
      input->ungetChar(c);
      return id;
    }
  }
  printError("Error parsing identifier");
  return QString::null;
}

/**
 * @short Skips a comment
 *
 * I'm already in the comment: if next char is * it is a / * * / comment, if its / its //.
 *
 * If other, then error. (return false)
 */
bool ConfigParser::skipComment(){
  char c;
  if (input->getChar(&c)){
    col++;
    if (c=='*'){ // /* */ comment
      bool beginEnd=false;
      while(input->getChar(&c)){
        col++;
        if (beginEnd && c=='/')
          return true;
        if (c=='*')
          beginEnd=true;
        else
          beginEnd=false;
        if (c=='\n'){
          line++;
          col=0;
        }
      }
    }
    else if (c=='/'){ // // comment
      while(input->getChar(&c)){
        col++;
        if (c=='\n'){
          line++;
          col=0;
          return true;
        }
      }
    }
    else if (c=='\n'){ 
      line++;
      col=0;
    }
    printError("Error parsing comment");
  }
  else
    printError("Error parsing comment, no valid comment start");
  // ERROR
  return false;

}

#define READ_TOKEN { if ( (t=readToken(r)) == UNKNOWN) return NULL; }
#define EXPECT(TOK) { if (t!=TOK){ printError(QString("Expecting " __STRING(TOK) " was '%1'").arg(r)); return NULL; } }
#define NEED(TOK) { READ_TOKEN; EXPECT(TOK); }

/**
 * @short Shows a pretty printed error, with filename, line and column and red human readable error.
 */
void ConfigParser::printError(const QString &e){
  qDebug("%s %d:%d \033[01;31m%s\033[00m",(char*)filename.toUtf8().data(),line,col,(char*)e.toUtf8().data());
  hasErrors=true;

  t=readToken(r); // skip myself as i will be an error for some time...
}


/**
 * @short Reads the positional and named parameters
 *
 * Sets the new values a positional and named.
 */
QString ConfigParser::readParameters(QStringList &positional, QMap<QString,QString> &named){
  Token t;
  QString r;
  QString id;

  READ_TOKEN;
  if (t==STRING){
    positional.append(r);
    READ_TOKEN;
    while(t==','){
      READ_TOKEN;
      if (t==IDENTIFIER || t==')'){ // now at named parameters, or end
        break;
      }
      EXPECT(STRING)
      positional.append(r);
      READ_TOKEN;
    }
  }
  if (t==IDENTIFIER){
    id=r;
    NEED('=')
    NEED(STRING);
    named[id]=r;
    READ_TOKEN;
    while(t==','){
      NEED(IDENTIFIER);
      id=r;
      NEED('=')
      NEED(STRING);
      named[id]=r;
      READ_TOKEN;
    }
  }
  return id;
}

/**
 * @short Returns the parsed root, or NULL is none.
 *
 * If NULL is returned, you can call error() to get an explanation.
 */
Module *ConfigParser::getRoot(){
  if (!input->isOpen()){
    printError("File not open");
    return NULL;
  }

  input->seek(0);

  READ_TOKEN;

  // Really read.
  return getModuleList();
}

/**
 * @short Just waits for a next module -> or end of chain ;
 */
Module *ConfigParser::getNextModule(){
  if (t==';'){
    READ_TOKEN;// Always ready
    return NULL;
  }
  if (t=='-'){
    NEED('>');
    READ_TOKEN;// Always ready
    return getModule();
  }
  printError("Unexpected error, expected -> or ;");
  return NULL;
}

/**
 * @short  It gets the last readen token, and leaves at the same place the new ones.
 */
Module *ConfigParser::getModule(){
  QString name;
  QStringList positionalParams;
  QMap<QString,QString> namedParams;
  Module *next;
  Module *me=NULL;

  switch(t){
    case IDENTIFIER:
      name=r;
      READ_TOKEN;  // maybe ( or [
      if (t=='(' || t=='['){
        readParameters(positionalParams, namedParams);
        READ_TOKEN; // always ready
      }
      if (!hasErrors){
        me=ModuleFactory::getModule(name,positionalParams,namedParams);
        if (!me)
          printError(QString("Cant construct module '%1'").arg(name));
      }
      else
        me=ModuleFactory::getModule("fake",positionalParams,namedParams);    
      break;
    case '[':
      me=getServerOrPath();
      READ_TOKEN; // always ready
      break;
    case '{':
      READ_TOKEN; // prepare next
      me=getModuleList();
      if (t!='}'){
        printError("Unbalanced brackets {}");
        if (me)
          delete me;
        return NULL;
      }
      READ_TOKEN; // always ready
      break;
    default:
      printError(QString("Expecting an IDENTIFIER, [ or {; got '%1'").arg((t<127) ? QString(t) : r));
      break;
  }
  if (me){
    next=getNextModule();
    me->setNextModule(next);
  }
  
  return me;
}


/**
 * @short Reads a small portion of the module chains. Actually between {}
 *
 * It gets the last readen token, and leaves at the same place the new ones.
 */
Module* ConfigParser::getModuleList(){
  ModuleList *list=new ModuleList();
  Module *tmod;

  while(t!='}' && t!=END_OF_FILE){ // until end or block or file
    tmod=getModule();  // Just read modules
    if (tmod==NULL)
      return NULL;
    list->append(tmod);
  }
  return list;
}


/**
 * @short Reads a small portion of the module chains. Actually between [ and ] (paths)
 *
 * It gets the last readen token, and leaves at the same place the new ones.
 */
Module* ConfigParser::getServerOrPath(){
  bool isServer;
  QString name;
  Module *me=NULL;

  READ_TOKEN;
  isServer=(t=='[');

  if (isServer)
    READ_TOKEN;

  name=r;

  NEED(']');
  if (isServer)
    NEED(']');

  if (!hasErrors){
    me=ModuleFactory::getModule(isServer ? "[[" : "[",QStringList(name),QMap<QString,QString>());
    if (!me)
        printError(QString("Cant construct module '%1'").arg(isServer ? "[[" : "["));
  }
  else
    me=ModuleFactory::getModule("fake",QStringList(name),QMap<QString,QString>());

  return me;
}

