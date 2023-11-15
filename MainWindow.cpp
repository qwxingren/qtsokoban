/*
 *  ksokoban - a Sokoban game for KDE
 *  Copyright (C) 1998  Anders Widell  <awl@hem.passagen.se>
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

#include <stdio.h>

#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>
#include <assert.h>
#include <qstring.h>
#include <qframe.h>
#include <QMessageBox>
#include <QSettings>

#include "MainWindow.h"
#include "PlayField.h"
#include "LevelCollection.h"
#include "StaticImage.h"

#define i18n(str) str
void
MainWindow::createCollectionMenu() {
  collection_ = new QMenu("collection menu", 0);
  _collectionGrp = new QActionGroup(this);
  _collectionGrp->setExclusive(true);
  
  connect(_collectionGrp, SIGNAL(triggered(QAction *)), this, SLOT(changeCollection(QAction *)));

  QAction *_action;
  for (int i=0; i<internalCollections_.collections(); i++) {
	  _action = collection_->addAction(internalCollections_[i]->name());
	  _action->setCheckable(true);
	  _collectionGrp->addAction(_action);
  }
  checkedCollection_ = 0;

  QSettings settings("Sokoban.ini", QSettings::IniFormat);

  int id = settings.value("settings/collection", 10).toInt();

  currentCollection_ = 0;
  for (int i=0; i<internalCollections_.collections(); i++) {
    if (internalCollections_[i]->id() == id) currentCollection_ = i;
  }

  changeToCollection(currentCollection_);
}

void MainWindow::about()
 {

   QString translatedTextAbout = tr(
        "<h3>About QSokoban</h3>"
        "<p>QSokoban is a game porting to Windows from  KSokoban</p>"
		"<p>                         by <a href=\"mailto:jia.jacky@gmail.com\">Jacky Jia</a></p>"
		"<p>License:GPL (c) 1998-2001</p>"
		"<p>More info see <a href=\"http://hem.passagen.se/awl/ksokoban/\">http://hem.passagen.se/awl/ksokoban/</a></p>"
		"<p>Source code <a href=\"https://github.com/qwxingren/qtsokoban\">https://github.com/qwxingren/qtsokoban</a></p>"
		"<p> </p>"
		"<p> </p>"
		"<h3>Update</h3>"
		"<p>1.save move after compeleted level</p>"
		"<p>2.play moving movie</p>"
		);
   QMessageBox *qsokoban_about = new QMessageBox(QMessageBox::NoIcon,QObject::tr("About QSokoban"),translatedTextAbout);
   qsokoban_about->setIconPixmap(playField_->imageData_->idk());
   qsokoban_about->show();
 }


MainWindow::MainWindow() : QMainWindow(0), 
	externalCollection_(0), 
	goToBM_(0),
	setBM_(0),
    bookmarkMenu_(0),
	animation_(0),
	collection_(0),
	game_(0),
	playField_(0)
{
  int i;
  QPixmap pixmap;

  //setEraseColor(QColor(0,0,0));
  QColor color = QColor(0, 0, 0);
  QPalette p = this->palette();
  p.setColor(QPalette::Window, color);
  this->setPalette(p);

  setWindowTitle(i18n("Qsokoban"));
  QSettings settings("Sokoban.ini", QSettings::IniFormat);
  int width = settings.value("Geometry/width", 750).toInt();
  int height = settings.value("Geometry/height", 562).toInt();
  resize(width, height);

  playField_ = new PlayField(this, "playfield");
  setCentralWidget(playField_);
  playField_->show();


  game_  = menuBar()->addMenu(i18n("&Game"));
  game_->addAction( i18n("&Load Levels..."), this, SLOT(loadLevels()));
  game_->addAction( i18n("Level &Movie"), this, SLOT(LevelMovie()),Qt::Key_M);
  game_->addAction( i18n("&Next Level"), playField_, SLOT(nextLevel()), Qt::Key_N);
  game_->addAction( i18n("&Previous Level"), playField_, SLOT(previousLevel()), Qt::Key_P);
  game_->addAction( i18n("Re&start Level"), playField_, SLOT(restartLevel()), Qt::Key_Escape);

  createCollectionMenu();
  game_->addMenu(collection_);

  //pixmap = NULL;//SmallIcon("undo");
  game_->addAction(i18n("&Undo"), playField_, SLOT(undo()),QKeySequence( /*(KStdAccel::undo()).toString()*/Qt::CTRL+Qt::Key_Z));
  //pixmap = NULL;//SmallIcon("redo");
  game_->addAction( i18n("&Redo"), playField_, SLOT(redo()), QKeySequence( /*(KStdAccel::redo()).toString()*/Qt::CTRL+Qt::SHIFT+Qt::Key_Z));
  //game_->insertSeparator();
  //pixmap = NULL;//SmallIcon("exit");
  game_->addAction( i18n("&Quit"), qApp, SLOT(closeAllWindows()), QKeySequence( /*(KStdAccel::quit()).toString()*/Qt::CTRL+Qt::Key_Q ));
  
  animation_ = menuBar()->addMenu(i18n("&Animation"));

 
  animationActionsGrp = new QActionGroup(this);
  animationActionsGrp->setExclusive(true);

  _actionSlow = new QAction(i18n("&Slow"), this);
  _actionSlow->setCheckable(true);
  connect(_actionSlow, &QAction::toggled, this, &MainWindow::animationSlow);
  _actionMedium = new QAction(i18n("&Medium"), this);
  _actionMedium->setCheckable(true);
  connect(_actionMedium, &QAction::toggled, this, &MainWindow::animationMedium);
  _actionFast = new QAction(i18n("&Fast"), this);
  _actionFast->setCheckable(true);
  connect(_actionFast, &QAction::toggled, this, &MainWindow::animationFast);
  _actionOff = new QAction(i18n("&Off"), this);
  _actionOff->setCheckable(true);
  connect(_actionOff, &QAction::toggled, this, &MainWindow::animationOff);
  checkedAnim_ = playField_->animDelay();
  if (checkedAnim_ == 0)_actionOff->setChecked(true);
  if (checkedAnim_ == 1)_actionFast->setChecked(true);
  if (checkedAnim_ == 2)_actionMedium->setChecked(true);
  if (checkedAnim_ == 3)_actionSlow->setChecked(true);

  animationActionsGrp->addAction(_actionSlow);
  animationActionsGrp->addAction(_actionMedium);
  animationActionsGrp->addAction(_actionFast);
  animationActionsGrp->addAction(_actionOff);
  animation_->addActions(animationActionsGrp->actions());
  updateAnimMenu(checkedAnim_,true);
 
  QAction *_tmpAction = NULL;
  bookmarkMenu_ = menuBar()->addMenu(i18n("&Bookmarks"));
  //pixmap = SmallIcon("bookmark_add");
 
  setBM_ = new QMenu("set bookmark menu", 0);
  _setBMActiongrp = new QActionGroup(this);

  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_1);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_2);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_3);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_4);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_5);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_6);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_7);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_8);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_9);
  _setBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL+Qt::Key_0);
  _setBMActiongrp->addAction(_tmpAction);

  setBM_->addActions(_setBMActiongrp->actions());
  connect(_setBMActiongrp, SIGNAL(triggered(QAction*)), this, SLOT(setBookmark(QAction*)));
  //connect(setBM_, SIGNAL(activated(int)), this, SLOT(setBookmark(int)));
  bookmarkMenu_->addMenu(setBM_);
  
 
  //pixmap = SmallIcon("bookmark");
  goToBM_ = new QMenu("go to bookmark menu", 0);
  _gotoBMActiongrp = new QActionGroup(this);

  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_1);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_2);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_3);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_4);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_5);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_6);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_7);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_8);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_9);
  _gotoBMActiongrp->addAction(_tmpAction);
  _tmpAction = new QAction(i18n("(unused)"), this);
  _tmpAction->setShortcut(Qt::CTRL + Qt::Key_0);
  _gotoBMActiongrp->addAction(_tmpAction);

  connect(_gotoBMActiongrp, SIGNAL(triggered(QAction*)), this, SLOT(goToBookmark(QAction*)));
  goToBM_->addActions(_gotoBMActiongrp->actions());
  bookmarkMenu_->addMenu( goToBM_);


  help_ = menuBar()->addMenu(i18n("&Help"));

  aboutQtAct = new QAction("About &Qt", this);
  aboutQtAct->setStatusTip("Show the Qt library's About box");
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
  
  aboutAct = new QAction("&About", this);
  aboutAct->setStatusTip("Show the application's About box");
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  help_->addAction(aboutAct);
  help_->addAction(aboutQtAct);

  for (i=1; i<=10; i++) {
    bookmarks_[i-1] = new Bookmark(i);
    updateBookmark(i);
  }

  setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
  QSettings settings("Sokoban.ini", QSettings::IniFormat);

  settings.setValue("Geometry/width", width());
  settings.setValue("Geometry/height", height());
  settings.setValue("settings/collection", internalCollections_[checkedCollection_]->id());

  settings.sync();
  for (int i=1; i<=10; i++) {
    delete bookmarks_[i-1];
  }


  delete externalCollection_;

  if(goToBM_)
	delete goToBM_;
  if(setBM_)
	delete setBM_;
  if(bookmarkMenu_)
	delete bookmarkMenu_;
  if(animation_)
	delete animation_;
  if(collection_)
	delete collection_;
  if(game_)
	delete game_;
  if(playField_)
	delete playField_;
}



void
MainWindow::focusInEvent(QFocusEvent *) {
  playField_->setFocus();
}

void
MainWindow::updateAnimMenu(int i,bool p) {

  if (!p) return;
  playField_->changeAnim(i);
  checkedAnim_ = i;
}
 

void
MainWindow::updateBookmark(int num) {
  int col = internalCollections_.toInternalId(bookmarks_[num-1]->collection());
  int lev = bookmarks_[num-1]->level();
  int mov = bookmarks_[num-1]->moves();

  if (col < 0 || lev < 0) return;

  QString name;
  if (col >= 0 && col < internalCollections_.collections())
    name = internalCollections_[col]->name();
  else
    name = i18n("(invalid)");
  QString l;
  l.setNum(lev+1);
  name += " #" + l;
  l.setNum(mov);
  name += " (" + l + ")";
  
  QAction *action = _setBMActiongrp->actions().at(num-1);
  if(action) action->setText(name);
  action = _gotoBMActiongrp->actions().at(num - 1);
  if (action) action->setText(name);
}

void
MainWindow::setBookmark(QAction *action) {
  //assert(id >= 1 && id <= 10);
  int id = _setBMActiongrp->actions().indexOf(action);
  assert(id >= 0 && id <= 9);
  if (id == -1)  return;
  playField_->setBookmark(bookmarks_[id]);
  updateBookmark(id+1);
}

void
MainWindow::goToBookmark(QAction* action) {
  int id = _gotoBMActiongrp->actions().indexOf(action);
  assert(id >= 0 && id <= 9);
  Bookmark *bm = bookmarks_[id];
  int collection = internalCollections_.toInternalId(bm->collection());
  int level = bm->level();

  if (collection < 0 || collection >= internalCollections_.collections()) return;
  LevelCollection* colPtr = internalCollections_[collection];
  if (colPtr == 0) return;
  if (level < 0 || level >= colPtr->noOfLevels()) return;
  if (level > colPtr->completedLevels()) return;

  playField_->setUpdatesEnabled(false);
  changeToCollection(collection);
  playField_->setUpdatesEnabled(true);
  playField_->goToBookmark(bookmarks_[id]);
}

void
MainWindow::changeCollection(QAction *action)
{
  int id = _collectionGrp->actions().indexOf(action);
  assert(id >= 0);
  
  checkedCollection_ = id;
  delete externalCollection_;
  externalCollection_ = 0;
  playField_->changeCollection(internalCollections_[id]);
}

void
MainWindow::changeToCollection(int id)
{
	QAction *action = _collectionGrp->actions().at(id);
	if (action)
		action->setChecked(true);
	checkedCollection_ = id;
	delete externalCollection_;
	externalCollection_ = 0;
	playField_->changeCollection(internalCollections_[id]);
}


void
MainWindow::loadLevels() {
 // KConfig *cfg=(KApplication::kApplication())->config();
 // cfg->setGroup("settings");
 // QString lastFile = cfg->readPathEntry("lastLevelFile");

  //KURL result = KFileDialog::getOpenURL(lastFile, "*", this, i18n("Load Levels From File"));
  //if (result.isEmpty()) return;

  //openURL(result);
}

void
MainWindow::LevelMovie() {
  game_->setEnabled(false);
  collection_->setEnabled(false);
  animation_->setEnabled(false);
  bookmarkMenu_->setEnabled(false);
  help_->setEnabled(false);
  PlayMovie();
  //if(!playthread)
  //  playthread = new MovieThread();
  //playthread->window = widget;

  //playthread->start();
  //playthread->wait();
  return;
}

void
MainWindow::openURL(QString _url) {
//  KConfig *cfg=(KApplication::kApplication())->config();

//   int namepos = _url.path().findRev('/') + 1; // NOTE: findRev can return -1
//   QString levelName = _url.path().mid(namepos);
  QString levelName = _url;

  QString levelFile;
/*  if (_url.isLocalFile()) {
    levelFile = _url.path();
  } else {
//     levelFile = locateLocal("appdata", "levels/" + levelName);
    if(!KIO::NetAccess::download( _url, levelFile ) )
	  return;
  }
*/
  LevelCollection *tmpCollection = new LevelCollection(levelFile, levelName);
//  KIO::NetAccess::removeTempFile(levelFile );

  if (tmpCollection->noOfLevels() < 1) {
    QMessageBox::warning(this, "sorry", i18n("No levels found in file"));
    delete tmpCollection;
    return;
  }
/*
  if (_url.isLocalFile()) {
	QSettings settings("Sokoban.ini", QSettings::IniFormat);
	settings.setValue("settings/lastLevelFile", _url.path());
  }
*/
  delete externalCollection_;
  externalCollection_ = tmpCollection;

  //collection_->setItemChecked(checkedCollection_, false);
  playField_->changeCollection(externalCollection_);


}

void
MainWindow::dragEnterEvent(QDragEnterEvent* event) {
//  event->accept(KURLDrag::canDecode(event));
}

void
MainWindow::dropEvent(QDropEvent* event) {
/*
  KURL::List urls;
  if (KURLDrag::decode(event, urls)) {
     kdDebug() << "MainWindow:Handling QUriDrag..." << endl;
     if (urls.count() > 0) {
         const KURL &url = urls.first();
         openURL(url);
     }
  }
  */
}
void
MainWindow::PlayMovie(){
  if (!playField_->playHistory())
    MovieEnd();
}


void
MainWindow::MovieEnd(){
  game_->setEnabled(true);
  collection_->setEnabled(true);
  animation_->setEnabled(true);
  bookmarkMenu_->setEnabled(true);
  help_->setEnabled(true);
  return;
}

void MainWindow::animationSlow(bool p)
{
	updateAnimMenu(3, p);
}
void MainWindow::animationMedium(bool p)
{
	updateAnimMenu(2, p);
}
void MainWindow::animationFast(bool p)
{
	updateAnimMenu(1, p);
}
void MainWindow::animationOff(bool p)
{
	updateAnimMenu(0, p);
}