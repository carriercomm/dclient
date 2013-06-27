/* edit.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#include <cctype>

#include <ndk/pen.hh>

#include <ndk/application.hh>
#include <ndk/edit.hh>
#include <ndk/packer.hh>

using ndk::input;

input::input (std::string const &label, panel *parent)
  : panel (parent)
  , label_ (label, this)
  , edit_ ("", this)
{
  events ()->on_system (slot (&input::at_system, this));
}

void
input::resize (int size)
{
  panel::resize (size, 1);
  size -= label_.width () + 1;
  if (size > 0)
    edit_.resize (size);
}

void
input::draw ()
{
  packer<pack::horizontal, pack::no_resizer> pack (this, align::center, 1);
  pack.add (&label_);
  pack.add (&edit_);
  pack.pack ();
  panel::draw ();
}

void
input::set_label (std::string const &str)
{
  label_.set_text (str);
}

void
input::set_text (std::string const &str)
{
  edit_.set_text (str);
}

void
input::set_mode (edit::echo_mode mode)
{
  edit_.set_mode (mode);
}

std::string const &
input::text () const
{
  return edit_.text ();
}

ndk::event::result
input::at_system (event const &ev)
{
  event::result res = event::ignored;

  if (!ev.sender_ && engine::enter_focus == ev.type_)
    edit_.events ()->system (ev);
  return res;
}
