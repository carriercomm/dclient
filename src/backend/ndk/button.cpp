/* button.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#include <ndk/pen.hh>
#include <ndk/application.hh>
#include <ndk/button.hh>
#include <ndk/palette.hh>

using ndk::mini_button;
using ndk::button;

namespace
{
  struct run_callback
  {
    /**< functor to run callback functions */
    template<typename CallbackT>
    void
    operator () (CallbackT &callback)
    {
      callback ();
    }
  };
}


mini_button::mini_button (std::string const &str, ndk::panel *parent, bool accept)
  : label (str, parent)
{
}

button::button (std::string const &str, panel *parent, bool accept)
  : label (str, parent)
  , state_ (engine::leave_focus)
{
  events ()->on_system (slot (&button::at_system, this));
}

void
button::set_color (ndk::pen &p)
{
  p.set_color (palette::instance ().find (colors::button_focused)->second);
}

/*
 * Note:
 * There are following reason to move widget's panel to the top:
 * ncurses has strange algorithm of moving cursor
 * if widget on the top, it's cursor is active and visible on the screen
 * I need this feature for the Line Edit
 */
ndk::event::result
button::at_system (event const &ev)              /**< system event processor */
{
  event::result res = event::ignored;

  if (!ev.sender_)
    {
      state_ = ev.type_;
      res = event::accepted;
      draw ();
    }

  return res;
}
