/* window.cc
 * This file is part of ncursesxx library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libncursesxx as the file COPYING.
 */
#include <ndk/window.hh>

#include <ndk/application.hh>

#include <stdexcept>

using ndk::window;

window::window ()
  : win_ (newwin (1, 1, 0, 0), delwin)
{
  if (!win_)
    throw std::runtime_error ("newwin call failed");
  app->enable_keypad (win ());
}

window::~window ()
{
}

void
window::set_scroll (bool res)
{
  scrollok (win (), res);      /* don't use ::, because scrollok may be a macro */
}

void
window::resize (int w, int h)
{
  return_unless (wresize (win (), h, w) == OK);
}

int
window::width () const
{
  int y, x;

  getmaxyx (win (), y, x);
  return x;
}

int
window::height () const
{
  int y, x;

  getmaxyx (win (), y, x);
  return y;
}

int
window::x () const
{
  int y, x;

  getbegyx (win (), y, x);
  return x;
}

int
window::y () const
{
  int y, x;

  getbegyx (win (), y, x);
  return y;
}

void
window::touch_line (int num, int count)
{
  touchline (win (), num, count);
}

void
ndk::copy (window &source, window &destination)
{
  return_unless (copywin (source.win (), destination.win (), 0, 0, 0, 0,
                          source.height () - 1, source.width () - 1, FALSE) == OK);
}
