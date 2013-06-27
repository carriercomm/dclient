/* text.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#include <ndk/color.hh>
#include <ndk/packer.hh>
#include <ndk/palette.hh>
#include <ndk/text.hh>

#include <ndk/pen.hh>

using ndk::text;

text::text (std::string const &title, ndk::panel *parent, wrap_type wrapping)
  : ndk::frame (title, parent)
  , x_ (0)
  , y_ (0)
  , wrapping_ (wrapping)
{
  pen pen (pad_);

  pen.fill (palette::instance ().find (colors::label)->second);

  events ()->on_keyboard (slot (&text::at_keyboard, this));
}

void
text::add_text (color c, std::string const &str)
{
  bool at_bottom = y_ + height () >= pad_.height ();

  /* calculate new height */
  pad_.resize ( wrapping_ == wrap
                  ? width () - 2
                  : std::max (pad_.width (), string_width (str) + 1)
              , std::max (height (), pen (pad_).y () + string_height (str))
              );
  ndk::pen pen (pad_);
  pen.set_color (c);
  pen.write (str);     /* add next text to the pad */

  if (at_bottom)
    while (down ())
      ;
}

void
text::draw ()     /**< draw frame on the panel */
{
  frame::draw ();
  pad_.draw (*this, x_, y_);      /* draw pad content in the viewport */
}

bool
text::right ()
{
  if (x_ + width () < pad_.width () - 1)
    {
      ++x_;
      draw ();
      return true;
    }
  return false;
}

bool
text::left ()
{
  if (x_ > 0)
    {
      --x_;
      draw ();
      return true;
    }
  return false;
}

bool
text::down ()
{
  if (y_ + height () < pad_.height ())
    {
      ++y_;
      draw ();
      return true;
    }
  return false;
}

bool
text::up ()
{
  if (y_ > 0)
    {
      --y_;
      draw ();
      return true;
    }
  return false;
}

ndk::event::result
text::at_keyboard (keyboard const &ev)
{
  switch (ev.code ())
    {
    case key::right:
    case key::l:
      right ();
      break;
    case key::left:
    case key::h:
      left ();
      break;
    case key::up:
    case key::k:
      up ();
      break;
    case key::down:
    case key::j:
      down ();
      break;
    default:
      return event::ignored;
    }

  pen (*this).commit ();
  emit (changed);

  return event::accepted;
}

int
text::string_height (std::string const &str)             /**< @return number of strings */
{
  int count = 0;

  if (wrapping_ == wrap)
    /* in wrap mode we need additional recalculations of the string height */
    count = str.length () / width () + (str.length () % width () != 0 ? 1 : 0);
  else
    /* not wrap mode: only calculate number of '\n' */
    count = std::count (str.begin (), str.end (), '\n');

  /* we also need an additional space below if line have '\n' at the end */
  if (*(str.end () - 1) == '\n')
    ++count;                                   /* grow width if nesessary */
  return count;
}

int
text::string_width (std::string const &str)             /**< @return maximum string length */
{
  std::string::const_iterator beg = str.begin ();
  std::string::const_iterator end = str.end ();
  ptrdiff_t max_len = 0;
  bool need_fix = true;

  do
    {
      std::string::const_iterator curr (find (beg, str.end (), '\n'));
      max_len = std::max (max_len,
                          distance (beg, curr) + ((need_fix) ? pen (pad_).x () : 0));
      beg = (curr == end) ? curr : (curr + 1);
      need_fix = false;
    }
  while (beg != end);

  return max_len;
}

void
text::resize (int w, int h)
{
  frame::resize (w, h);
  /* grow size of the text if we need it */
  pad_.resize (std::max (pad_.width (), w + 1), std::max (pad_.height (), h));
}

void
text::clear ()
{
  pen (pad_).clear ();
  x_ = 0;
  y_ = 0;
  /* truncate buffer to viewport size */
  pad_.resize (this->width (), this->height ());
  draw ();
}

void
text::set_text (color c, std::string const &str)
{
  clear ();
  add_text (c, str);
  draw ();
}

int
text::length () const
{
  return pad_.height ();
}

int
text::last_line () const
{
  return y_ + height ();
}
