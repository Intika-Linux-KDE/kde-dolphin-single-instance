/***************************************************************************
*   Copyright (C) 2019 by intika <intika@librefox.org>                    *
*   Copyright (C) 2018 by intika <intika@librefox.org>                    *
*   Copyright (C) 2016 by Fady S. Ghatas <fady@titrias.com>               *
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

#ifndef DOLPHIN_SINGLEWINDOW_H
#define DOLPHIN_SINGLEWINDOW_H

#include "dolphinmainwindow.h"
#include "global.h"
#include <QApplication>
#include <QIcon>
#include <QList>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QPointer>
#include <QUrl>
#include <QWidget>
#include <QtNetwork/QLocalServer>
#include <kxmlguiwindow.h>

/**
 * @short Single window for Dolphin.
 *
 * Handles the menus, toolbars and Dolphin views.
 */
class DolphinSingleWindow : public DolphinMainWindow {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.kde.dolphin.SingleWindow")
  Q_PROPERTY(int id READ getId SCRIPTABLE true)

public:
  DolphinSingleWindow();
  virtual ~DolphinSingleWindow();
  QLocalServer *server;
  QString serverName;
  static DolphinSingleWindow *window;
  static DolphinSingleWindow *getWindow();
  /**
   * Open the list of urls in the single instance window
   * @param urls URLs of directories to open.
   */
  static void open(QList<QUrl> urls);
  /**
   * Create new Server that can receive new tab creation requests.
   */
  void createNewServer();

public slots:
  /**
   * Server Event Listener
   */
  void hookLocalConnection();
};
#endif // DOLPHIN_SINGLEWINDOW_H