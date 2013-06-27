/* frame.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */

#include <ndk/pen.hh>
#include <ndk/frame.hh>
#include <ndk/palette.hh>


ndk::frame::frame (std::string const &title, panel *parent)
  : panel (parent)
  , title_ (title)
  , active_ (false)
{
  events ()->on_system (slot (&frame::at_system, this));
}

void
ndk::frame::draw ()
{
  panel::draw ();

  pen p (*this);
  p.clear ();

  p.set_color (palette::instance ().find (active_ ? colors::frame1 : colors::frame2)->second);
  p.frame ();
  if (!title_.empty ())
    p.title (title_.substr (0, width () - 2));
  p.set_color (palette::instance ().find (colors::frame2)->second);
}

int
ndk::frame::padding () const
{
  return 1;
}

std::string const &
ndk::frame::title () const
{
  return title_;
}

void
ndk::frame::set_title (std::string const &title)
{
  title_ = title;
}

ndk::event::result
ndk::frame::at_system (ndk::event const &ev)
{
  event::result res = event::ignored;

  if (!ev.sender_)
    {
      active_ = engine::enter_focus == ev.type_;
      prepare ();
      res = event::accepted;
    }

  return res;
}
