#include "live.h"

#include <ndk/pen.hh>

#include <ndk/color.hh>
#include <ndk/pad.hh>
#include <ndk/window.hh>


static bool
is_bold (WINDOW *win)
{
  short pair;
  attr_t attrs;

  wattr_get (win, &attrs, &pair, NULL);

  return attrs & WA_BOLD;
}


using ndk::pen;

void
pen::write (WINDOW *win, char const *str, int len)
{
#if LIVE
  waddnstr (win, str, len);
#endif
}

void
pen::write (WINDOW *win, std::string const &str)
{
  write (win, str.data (), str.length ());
}

void
pen::write (WINDOW *win, wchar_t wc)
{
#if LIVE
  waddnwstr (win, &wc, 1);
#endif
}

void
pen::write (WINDOW *win, color c, wchar_t wc)
{
#if LIVE
  cchar_t ch = { c.attr (), { wc } };
  wadd_wch (win, &ch);
#endif
}

wchar_t const &pen::ltitle   (bool bold) { static wchar_t const s[2] = { L'┤', L'╣' }; return s[bold]; }
wchar_t const &pen::rtitle   (bool bold) { static wchar_t const s[2] = { L'├', L'╠' }; return s[bold]; }
wchar_t const &pen::ulcorner (bool bold) { static wchar_t const s[2] = { L'┌', L'╔' }; return s[bold]; }
wchar_t const &pen::llcorner (bool bold) { static wchar_t const s[2] = { L'└', L'╚' }; return s[bold]; }
wchar_t const &pen::urcorner (bool bold) { static wchar_t const s[2] = { L'┐', L'╗' }; return s[bold]; }
wchar_t const &pen::lrcorner (bool bold) { static wchar_t const s[2] = { L'┘', L'╝' }; return s[bold]; }
wchar_t const &pen::h_line   (bool bold) { static wchar_t const s[2] = { L'─', L'═' }; return s[bold]; }
wchar_t const &pen::v_line   (bool bold) { static wchar_t const s[2] = { L'│', L'║' }; return s[bold]; }

pen::pen (window &win)
: win_ (win.win ())
{
}

pen::pen (pad &pad)
: win_ (pad.win ())
{
}

bool
pen::move (int x, int y)
{
  return_val_unless (wmove (win_, y, x) == OK, false);
  return true;
}

void
pen::commit ()
{
  return_unless (wnoutrefresh (win_) == OK);
}

int
pen::x () const
{
  return getcurx (win_);
}

int
pen::y () const
{
  return getcury (win_);
}

#if 0
int
pen::width () const
{
  return getmaxx (win_);
}

int
pen::height () const
{
  return getmaxy (win_);
}
#endif

void
pen::clear ()
{
  return_unless (werase (win_) == OK);
}

void
pen::fill (color col)
{
  return_unless (wbkgd (win_, COLOR_PAIR (col.num ())) == OK);
}

void
pen::set_color (color c)
{
  /* what should I use here: wattrset or wattr_set */
  return_unless (wattr_set (win_, c.attr (), c.num (), 0) == OK);
}

void
pen::frame ()
{
  return_unless (move (0, 0));

  bool const bold = is_bold (win_);

  int maxx, maxy;
  getmaxyx (win_, maxy, maxx);

  return_unless (maxx > 1 && maxy > 1);

  write (ulcorner (bold));
  for (int i = 1; i < maxx - 1; i++)
    write (h_line (bold));
  write (urcorner (bold));

  for (int i = 1; i < maxy; i++)
    {
      write (v_line (bold));
      return_unless (move (0, i));
    }

  write (llcorner (bold));
  for (int i = 1; i < maxx - 1; i++)
    write (h_line (bold));
  write (lrcorner (bold));

  for (int i = 1; i < maxy - 1; i++)
    {
      return_unless (move (maxx - 1, i));
      write (v_line (bold));
    }
}

void
pen::title (std::string const &title)
{
  bool const bold = is_bold (win_);

  return_unless (move (2, 0));
  write (ltitle (bold));
  write (title);
  write (rtitle (bold));
}

void
pen::write (std::string const &str)
{
  write (win_, str);
}

void
pen::write (wchar_t wc)
{
  write (win_, wc);
}

void
pen::write (color c, wchar_t wc)
{
  write (win_, c, wc);
}
