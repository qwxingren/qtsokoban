/*
 *  ksokoban - a Sokoban game for KDE
 *  Copyright (C) 1998  Anders Widell  <d95-awi@nada.kth.se>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QMainWindow>
#include "Bookmark.h"
#include "InternalCollections.h"

class QMenuBar;
class PlayField;
class QMenu;
class QFocusEvent;
class QDragEnterEvent;
class QDropEvent;
class LevelCollection;
class QActionGroup;
class QAction;

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

  void openURL(QString _url);

public slots:
  void changeCollection(QAction *action);
  void updateAnimMenu(int id,bool p);
  void setBookmark(QAction* action);
  void goToBookmark(QAction *action);
  void PlayMovie();
  void loadLevels();
  void LevelMovie();
  void MovieEnd();
  void about();
  void animationSlow(bool p);
  void animationMedium(bool p);
  void animationFast(bool p);
  void animationOff(bool p);


protected:
  void focusInEvent(QFocusEvent*);
  void createCollectionMenu();
  virtual void dragEnterEvent(QDragEnterEvent*);
  virtual void dropEvent(QDropEvent*);

private:
  InternalCollections internalCollections_;
  LevelCollection *externalCollection_;
  PlayField       *playField_;
  Bookmark        *bookmarks_[10];
  int              currentCollection_;


  QMenu      *game_;
  QMenu      *collection_;
  QMenu      *animation_;
  QMenu      *bookmarkMenu_;
  QMenu      *setBM_;
  QMenu      *goToBM_;
  QMenu      *help_;
  QAction *aboutQtAct;
  QAction *aboutAct;
  QAction *_actionSlow;
  QAction *_actionMedium;
  QAction *_actionFast;
  QAction *_actionOff;
  QActionGroup *animationActionsGrp;
  QActionGroup *_setBMActiongrp;
  QActionGroup *_gotoBMActiongrp;
  QActionGroup *_collectionGrp;
  int              checkedCollection_;
  int              checkedAnim_;

  void updateBookmark(int num);
  void changeToCollection(int id);

};

extern MainWindow *widget;

#endif  /* MAINWINDOW_H */
