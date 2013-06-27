/* application.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */

#include <ndk/application.hh>

#include <ndk/color.hh>
#include <ndk/keymap.hh>
#include <ndk/palette.hh>
#include <ndk/panel.hh>

#include <term.h>

#include <clocale>

using ndk::application;

namespace std
{
  template<>
  size_t
  hash<ndk::colors::type>::operator () (ndk::colors::type type) const
  {
    return hash<int> () (type);
  }
}

application *ndk::app;

application::application ()
{
  /*
   * init ncurses enviroment
   * We use newterm/delscreen instead of initscr to reduce number of memory leaks
   */
  char const *name;

  if ((name = getenv ("TERM")) == NULL || !*name)
    name = "unknown";
  screen_ = newterm (const_cast<char *> (name), stdout, stdin);

  if (!screen_)
    throw std::runtime_error ("newterm failed");

  def_prog_mode ();
  return_unless (cbreak () == OK);
  return_unless (noecho () == OK);
  return_unless (nonl () == OK);
  return_unless (notimeout (stdscr, true) == OK);

  if ((has_colors_ = ::has_colors ()))
    return_unless (start_color () == OK);

  mousemask (ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
  cursor_off ();      /* turn off cursor, by default */

  enable_keypad (stdscr);
}

static void
init_color_pairs ()
{
  for (short fg = 0; fg < 8; fg++)
    for (short bg = 0; bg < 8; bg++)
      return_unless (init_pair (fg * 8 + bg + 1, fg, bg) == OK);
}

void
application::init_palette ()
{
  init_color_pairs ();

  palette::obj_type &p = palette::instance ();

  p.insert ({ colors::button_default,   { COLOR_WHITE, COLOR_BLACK            } });
  p.insert ({ colors::label,            { COLOR_WHITE, COLOR_BLACK            } });
  p.insert ({ colors::panel,            { COLOR_WHITE, COLOR_BLACK            } });
  p.insert ({ colors::title,            { COLOR_WHITE, COLOR_BLUE,  WA_BOLD   } });
  p.insert ({ colors::frame1,           { COLOR_WHITE, COLOR_BLUE,  WA_BOLD   } });
  p.insert ({ colors::frame2,           { COLOR_WHITE, COLOR_BLACK            } });
  p.insert ({ colors::button_focused,   { COLOR_BLACK, COLOR_WHITE            } });
  p.insert ({ colors::button_selected,  { COLOR_WHITE, COLOR_WHITE, WA_BOLD   } });
  p.insert ({ colors::minibutton,       { COLOR_WHITE, COLOR_BLACK            } });
  p.insert ({ colors::edit_active,      { COLOR_WHITE, COLOR_BLACK, WA_BOLD   } });
  p.insert ({ colors::edit_inactive,    { COLOR_WHITE, COLOR_BLACK            } });
}

application::~application ()
{
  endwin ();
  delscreen (screen_);
}

int
application::width () const
{
  return COLS;
}

int
application::height () const
{
  return LINES;
}

void
application::cursor_on ()
{
  /* first try to made cursor super visible, otherwise made it normal */
  curs_set (cursor_visible ? 2 : 1);
}

void
application::cursor_off ()
{
  curs_set (0);      /* turn on cursor */
}

ndk::engine *
application::events ()
{
  return &events_;
}

void
application::update ()
{
  events_.update ();
}

void
application::process (int ch)
{
  events_.process (ch);
}

void
application::in_focus (panel &other)
{
  events_.in_focus (other.events ());
}

void
application::enable_keypad (WINDOW *win)
{
  if (!has_keymap ())
    return_unless (keypad (win, true) == OK);
}
