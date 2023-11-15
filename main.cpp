/*
 *  ksokoban - a Sokoban game for KDE
 *  Copyright (C) 1998-2000  Anders Widell  <awl@passagen.se>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <QApplication>
#include "MainWindow.h"


static const char description[] = "The japanese warehouse keeper game";

static const char version[] = "0.4.2";

MainWindow *widget;

int
main (int argc, char **argv)
{

  //QCoreApplication::setOrganizationName("IDK");
  //QCoreApplication::setOrganizationDomain("IDK.com");
  //QCoreApplication::setApplicationName("Sokoban");
  QApplication app(argc, argv);
  QApplication::setColorSpec(QApplication::ManyColor);

 

  widget = new MainWindow();
  app.setActiveWindow(widget);
  widget->show();

  QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

  int rc = app.exec();

  delete widget;

  return rc;
}
