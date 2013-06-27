/* label.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#include <ndk/label.hh>
#include <ndk/palette.hh>

using ndk::label;

label::label (std::string const &str, ndk::panel *parent)
  : panel (parent)
  , text_ (str)
{
  if (!str.empty ())
    panel::resize (str.length (), 1);
  events ()->set_accept (false);     /* this widget doesn't accept focus */
}

void
label::set_text (std::string const &str)
{
  text_ = str;
}

std::string const &
label::text () const
{
  return text_;
}

void
label::draw ()
{
  draw (this->text_, 0);      /* simple drawing of the text */
}

void
label::draw (std::string const &str, std::string::size_type begin)
{
  pen p (*this);

  set_color (p);
  return_unless (p.move (0, 0));
  std::string out (str.substr (begin, this->width ()));
  if (int (out.length ()) < this->width ())
    out.append (this->width () - out.length (), ' ');
  p.write (out);
}

void
label::set_color (pen &p)
{
  p.set_color (palette::instance ().find (colors::label)->second);
}
