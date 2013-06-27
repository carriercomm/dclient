/* edit.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#include <ndk/application.hh>
#include <ndk/edit.hh>
#include <ndk/packer.hh>
#include <ndk/palette.hh>

#include <ndk/pen.hh>

#include <cctype>


using ndk::edit;

edit::edit (std::string const &str, panel *parent, bool accept)
  : button (str, parent, accept)
  , current_ (0)
  , begin_ (0)
  , mode_ (normal)
{
  events ()->on_keyboard (slot (&edit::at_keyboard, this));
  events ()->on_system (slot (&edit::at_system, this));
}

void
edit::resize (int size)
{
  button::resize (size, 1);
}

void
edit::set_text (std::string const &str)
{
  label::set_text (str);
  current_ = text_.length ();
  draw ();
}

void
edit::set_mode (echo_mode mode)
{
  mode_ = mode;
}

void
edit::change_color ()
{
  pen (*this).set_color (palette::instance ().find (
                           state_ == engine::enter_focus
                             ? colors::edit_active
                             : colors::edit_inactive
                         )->second);
}

void
edit::draw ()
{
  /* first process flags */
  std::string out;

  switch (mode_)
    {
    case normal:
      out.assign (text ());
      break;
    case no_echo:
      out.assign ("");
      break;
    case password:
      out.assign (std::string (text ().size (), '*'));
      break;
    default:
      break;
    }

  if (begin_ > out.length ())
    begin_ = out.length ();

  label::draw (out, begin_);    /* and draw */
  set_cursor ();                /* and finally update cursor position */
  pen (*this).commit ();        /* commit changes to the screen */
}

ndk::event::result
edit::at_system (event const &ev)
{
  event::result res = event::ignored;

  if (!ev.sender_)
    {
      if (engine::enter_focus == ev.type_)
        {
          app->cursor_on ();
          set_cursor ();          /* only set cursor position, don't need full redraw */
          res = event::accepted;
        }
      else if (engine::leave_focus == ev.type_)
        {
          app->cursor_off ();
          res = event::accepted;
        }
    }
  return res;
}

ndk::event::result
edit::at_keyboard (keyboard const &ev)
{
  event::result res = event::ignored;
  unsigned int pos = current_;

  switch (ev.code ())
    {
    case key::right:
      (scroll_right ()) ? draw () : set_cursor ();
      res = event::accepted;
      break;
    case key::left:
      (scroll_left ()) ? draw () : set_cursor ();
      res = event::accepted;
      break;
    case key::backspace:
      --pos;           /* try to go to previous position */
      if (pos < text_.length ())
        {
          erase (pos);
          scroll_left ();              /* scroll all view to the left */
          draw ();              /* we always need full redraw */
          res = event::accepted;
        }
      break;
    case key::dc:
      if (pos < text_.length ())
        {
          erase (pos);
          draw ();              /* we always need full redraw */
          res = event::accepted;
        }
      break;
    case key::ctrl_u:
       if (pos)
         {
           /* delete all characters up to the current */
           text_ = text_.substr (pos, text_.length () - pos);
           current_ = 0;
           recalibrate (); // TODO: ? draw () : set_cursor (); // or always draw?
           draw ();
           pos = 0;
         }
      res = event::accepted;
      break;
    case key::home:
      begin_ = 0;
      current_ = 0;
      draw ();
      res = event::accepted;
      break;
    case key::end:
      current_ = text_.length ();
      (recalibrate ()) ? draw () : set_cursor ();
      res = event::accepted;
      break;
    case key::enter:
      on_commit_ (text_);
      break;
    case key::tab:                                 /* don't beep on tab */
      break;
    default:                                                                /* enter key to the string */
      if (std::isprint (int (ev.code ())))
        {
          insert (pos, char (ev.code ()));
          scroll_right ();
          draw ();                  /* we always need full redraw */
          res = event::accepted;
        }
#if 0
      else
        throw ev.code ();
#endif
      break;
    }

  return res;
}

bool
edit::scroll_right ()
{
  if (current_ < text_.length ())
    ++current_;                                    /* increase current */
  /* recalibrate begin, decide are we need to redraw */
  return recalibrate ();
}

bool
edit::scroll_left ()
{
  if (current_ > 0)
    --current_;                       /* decrease current */
  /* recalibrate begin, decide are we need to redraw */
  return recalibrate ();
}

bool
edit::recalibrate ()
{
  bool res = false;

  if ((begin_ < current_) && ((current_ - begin_) > (width () - 1)))
    {
      /* may be cursor is over(after) edit screen? */
      begin_ = current_ - width () + 1;
      res = true;
    }
  else if (begin_ > current_)
    {
      /* may be cursor is over(before) edit screen? */
      begin_ = current_;
      res = true;
    }
  return res;
}

void
edit::set_cursor ()
{
  /* always move to zero position in no_echo mode */
  return_unless (pen (*this).move (mode_ != no_echo ? current_ - begin_ : 0, 0));
  pen (*this).commit ();
}

void
edit::insert (int pos, char sym)
{
  text_.insert (text_.begin () + pos, sym);           /* default mode is insert mode */
}

void
edit::erase (int pos)
{
  text_.erase (text_.begin () + pos);
}
