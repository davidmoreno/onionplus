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

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDateTime>
#include <QDir>

#include "client.h"
#include "response.h"
#include "module.h"

using namespace Onion;

Client::Client(QTcpSocket *client, Module *root) : request(client), root(root){
  atHeader=2;
  fromModule=NULL;

  connect(client,SIGNAL(readyRead()), this, SLOT(readRequest()));
  connect(client,SIGNAL(bytesWritten(qint64)),
          this,SLOT(readAndWrite()));

  //qDebug("%s:%d Client",__FILE__,__LINE__);
  readRequest();
}


Client::~Client(){
  // connected deleteLater to disconnect at daemon.cpp.
  //qDebug("%s:%d ~Client",__FILE__,__LINE__);
  delete fromModule;
}

/**
 * @short reads request from the client
 *
 * Reads all bytes needed for the request, and sets the request handler.
 */
void Client::readRequest(){
  qint64 len;
  char line[1024];

  QTcpSocket *client=dynamic_cast<QTcpSocket*>(request.client());
  while(client->canReadLine()){
    len=client->readLine(line,sizeof(line)/sizeof(line[0]));
    QString s;
    foreach(QString ss,QString(line).split('\n')){
      s=ss.simplified();
      if (atHeader){
        if (s=="") // Always looking for header end...
          atHeader--;
        else
          atHeader=2;

        request.addHeader(s);
      }
      if (!atHeader){
        processPetition();
        return;
      }
    }
  }
}

#define QS(str) ((char*)str.toUtf8().data())


/**
 * @short Calls the ProcessorFatory to perform the real calls.
 */
void Client::processPetition(){
  response=Response();
  bytesSent=0;

  if (root){
    if (fromModule)
      delete fromModule;
    fromModule=root->process(request,response);
  }

  //qDebug("%s:%d processing by %s",__FILE__,__LINE__,fromModule ? fromModule->metaObject()->className() : "NULL");
  if (!fromModule){
    response.setStatus(500);
    request.client()->write(response.headerAsByteArray());
    qDebug("%s:%d nobody could response your petition!",__FILE__,__LINE__);
    request.client()->write("<h1>404 - not found</h1>");
    request.client()->close();
    return;
  }
  else{
    qDebug("%s %s %s (to %s (%s) from %s, %d bytes)",
      QS(QDateTime::currentDateTime().toString(Qt::ISODate)),
      QS(request.getMethod()),
      QS(request.getFullPath()),
      QS(request.get("Host")),
      QS(request.get("Host-IP")),
      QS(request.client()->peerAddress().toString()),
      response.getHeader("Content-Length").toInt());
  }

  request.client()->write(response.headerAsByteArray());

  readAndWrite();
}

/**
 * @short Takes a little bit of data from input, and writes to output.
 *
 * Doing it here allows me to be multiconnection on single threads.
 */
void Client::readAndWrite(){
  if (!fromModule)
    return;
  //qDebug("%s:%d readAndWrite (continue? %d)",__FILE__,__LINE__,!fromModule->atEnd());
  if (response.getLength()<=bytesSent){
    //qDebug("%s:%d done: %d %ld",__FILE__,__LINE__,response.getHeader("Content-Length").toInt(),bytesSent);
    fromModule->close();
  }

  if (fromModule->atEnd() || !fromModule->isOpen()){ // Do nothing. Hangs this connection; maybe more petitions later.
    if (response.getHeader("connection").toLower()=="close" || 
        request.get("connection").toLower()!="keep-alive" || 
        response.getHeader("keep-alive").isEmpty()) {
       qDebug("%s:%d close",__FILE__,__LINE__);
      request.client()->disconnect();
    }
    else{ // Ready to get another...
      qDebug("%s:%d keep alive",__FILE__,__LINE__);
      atHeader=2;
      request.clean();
    }
    return;
  }

  while(request.client()->bytesToWrite()<=0){
    QByteArray output;
    output=fromModule->read(2*1024);
//     qDebug("%s:%d isOpen? %d %d %s",__FILE__,__LINE__, fromModule->isOpen(), fromModule->atEnd(), output.data());
    bytesSent+=request.client()->write(output); // buffer size should be tweakable.
  }
//   qDebug("%s:%d done",__FILE__,__LINE__);
}
