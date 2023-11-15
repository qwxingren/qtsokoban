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

#include <QList.h>

#include "History.h"
#include "Move.h"
#include "MoveSequence.h"
#include "LevelMap.h"

History::History() {
}


void
History::add(Move *_m) {
  //future_.clear();
  while (!future_.isEmpty())
	  delete future_.takeFirst();
  past_.append(_m);
}

void
History::clear() {
	while (!past_.isEmpty())
		delete past_.takeFirst();
	while (!future_.isEmpty())
		delete future_.takeFirst();
  //past_.clear();
  //future_.clear();
}

void
History::save(QString &_str) {
  Move *m = 0;
  if (!past_.isEmpty()) {
    m = past_.first();
  }
  unsigned int i = 0;
  while (m != 0) {
	i++;
    m->save(_str);
	if (i >= past_.size()) {
	  m = 0;
	}
	else {
	  m = past_.at(i);
	}
  }
  _str += '-';

  m = 0;
  i = 0;
  if (!future_.isEmpty()) {
	m = future_.first();
  }
  while (m != 0) {
	i++;
	m->save(_str);
	if (i >= future_.size()) {
	  m = 0;
	}
	else {
	  m = future_.at(i);
    }
  }
}

const char *
History::load(LevelMap *map, const char *_str) {
  Move *m;
  int x = map->xpos();
  int y = map->ypos();

  clear();
  while (*_str != '\0' && *_str != '-') {
    m = new Move(x, y);
    _str = m->load(_str);
    if (_str == 0) return 0;
    x = m->finalX();
    y = m->finalY();
    past_.append(m);
    if (!m->redo(map)) {
      return 0;
    }
  }
  if (*_str != '-') return 0;

  _str++;
  while (*_str != '\0') {
    m = new Move(x, y);
    _str = m->load(_str);
    if (_str == 0) return 0;
    x = m->finalX();
    y = m->finalY();
    future_.append(m);
  }

  return _str;
}

bool
History::redo(LevelMap *map) {
  if (future_.isEmpty()) return false;

  Move *m=future_.takeAt(0);
  past_.append(m);
  return m->redo(map);
}

MoveSequence *
History::deferRedo(LevelMap *map) {
  if (future_.isEmpty()) return 0;

  Move *m=future_.takeAt(0);
  past_.append(m);
  return new MoveSequence(m, map);
}

bool
History::undo(LevelMap *map) {
  if (past_.isEmpty()) return false;

  Move *m = past_.takeLast();
  future_.insert(0, m);
  return m->undo(map);
}

MoveSequence *
History::deferUndo(LevelMap *map) {
  if (past_.isEmpty()) return 0;

  Move *m = past_.takeLast();
  future_.insert(0, m);
  return new MoveSequence(m, map, true);
}
