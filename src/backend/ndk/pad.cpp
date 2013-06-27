/* pad.cc
 * This file is part of ncursesxx library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libncursesxx as the file COPYING.
 */
#include <ndk/pad.hh>

#include <ndk/application.hh>
#include <ndk/window.hh>

#include <stdexcept>

using ndk::pad;

pad::pad ()
  : pad_ (newpad (1, 1), delwin)
{
  if (!pad_)
    throw std::runtime_error ("newpad failed");
  app->enable_keypad (win ());
}

pad::~pad ()
{
}

void
pad::resize (int w, int h)
{
  return_unless (wresize (win (), h, w) == OK);
}

int
pad::width () const
{
  int x, y;

  getmaxyx (win (), y, x);
  return x;
}

int
pad::height () const
{
  int x, y;

  getmaxyx (win (), y, x);
  return y;
}

void
pad::draw (window &viewport, int x, int y)
{
  return_unless (copywin (win (), viewport.win (), y + 1, x, 1, 1,
                          viewport.height () - 2, viewport.width () - 2, FALSE) == OK);
}
