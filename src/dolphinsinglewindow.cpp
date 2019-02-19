/***************************************************************************
*   Copyright (C) 2016 by Fady S. Ghatas @ TiTrias                        *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
***************************************************************************/

#include "dolphinsinglewindow.h"
#define TIME_OUT (1000)
#define SERVER_NAME "SingleDolphin"
DolphinSingleWindow *DolphinSingleWindow::window = 0;

namespace {
// Used for GeneralSettings::version() to determine whether
// an updated version of Dolphin is running.
const int CurrentDolphinVersion = 200;
}

void DolphinSingleWindow::open(QList<QUrl> urls) {
  QLocalSocket socket;
  socket.connectToServer(SERVER_NAME);
  qDebug() << "Trying to connect to server";
  int result = socket.waitForConnected(TIME_OUT);
  qDebug() << result;
  if (result) {
    qDebug() << "Connected To Server";
    // send a message to the server
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << urls;
    out.device()->seek(0);
    socket.write(block);
    socket.flush();
    qDebug() << "Client to Server : Open New Tab";
    qDebug() << urls;
    qDebug() << "Quitting";
    return;
  } else {
    DolphinSingleWindow *window = getWindow();
    window->openDirectories(urls, false);
    window->show();
    window->createNewServer();
    //Restore window if minimized
    window->showNormal();
  }
}

DolphinSingleWindow *DolphinSingleWindow::getWindow() {
  if (!window) {
    qDebug() << "Creating the Window";
    window = new DolphinSingleWindow();
  }
  return window;
}
DolphinSingleWindow::DolphinSingleWindow() { createNewServer(); }

DolphinSingleWindow::~DolphinSingleWindow() {}

void DolphinSingleWindow::hookLocalConnection() {
  qDebug() << "Hook";
  QLocalSocket *socket = server->nextPendingConnection();
  if (socket) {
    socket->waitForReadyRead(2 * TIME_OUT);
    QList<QUrl> message;
    QDataStream in(socket);
    in >> message;
    qDebug() << "Server Received new Message";
    qDebug() << message;
    delete socket;
    QList<QUrl> urls;
    if (urls.isEmpty()) {
      // We need at least one URL to open Dolphin
      urls.append(Dolphin::homeUrl());
    }

    window->openDirectories(message, false);
    //Restore window if minimized
    window->showNormal();
  }
}
void DolphinSingleWindow::createNewServer() {
  server = new QLocalServer(this);
  connect(server, SIGNAL(newConnection()), this, SLOT(hookLocalConnection()));
  if (!server->listen(SERVER_NAME)) {
    if (server->serverError() == QAbstractSocket::AddressInUseError) {
      qDebug() << "Server Address is in use, Removing it =D";
      QLocalServer::removeServer(SERVER_NAME); // <-- A key
      server->listen(SERVER_NAME);             // Listen again
    }
  }
  qDebug() << "Server Address is Set";
}
