/* terminal.cc
 * This file is part of ncursesxx library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libncursesxx as the file COPYING.
 */
#include <ndk/terminal.hh>

#include <iostream>

#include <stdexcept>

using ndk::terminal;

static TERMINAL *
topen (int fd, char const *termname)
{
  /**< open new terminal descriptor */

  TERMINAL *new_term = 0;
  TERMINAL *prev_term = cur_term;

  cur_term = 0;
  int erret;
  /* mutex up here */
  if (setupterm (const_cast<char *> (termname), fd, &erret) != OK)
    {
      switch (erret)
        {
        case 1: throw std::runtime_error ("terminal is hardcopy");
        case 0: throw std::runtime_error ("unknown terminal");
        case -1: throw std::runtime_error ("terminfo database not found");
        default: throw std::runtime_error ("unknown error code");
        }
    }
  new_term = cur_term;
  set_curterm (prev_term);
  /* mutex down here */
  return new_term;
}

static void
tclose (TERMINAL *term)
{
  /**< close terminal descriptor */
  if (!term)
    return;
  del_curterm (term);
}

static char const *
ttigetstr (TERMINAL *term, char const *capname)
{
  /**< get string capability for the specified terminal */
  /* mutex up here */
  TERMINAL *prev_term = set_curterm (term);
  char const *value = tigetstr (const_cast<char *> (capname));

  set_curterm (prev_term);
  /* mutex down here */

  if (value == reinterpret_cast<char const *> (-1))            /* FIXME: made assertion here */
    throw std::runtime_error ("wrong string capability");
  return value;
}

static int
ttigetnum (TERMINAL *term, char const *capname)
{
  /**< get numeric capability for the specified terminal */
  /* mutex up here */
  TERMINAL *prev_term = set_curterm (term);
  int value = tigetnum (const_cast<char *> (capname));

  set_curterm (prev_term);
  /* mutex down here */

  if (value == -2)              /* FIXME: made assertion here */
    throw std::runtime_error ("wrong numeric capability");
  return value;
}

static int
tvidputs (TERMINAL *term, chtype mode, int (*putc)(int))
{
  /**< change attributes of the specified terminal */
  /* mutex up here */
  TERMINAL *prev_term = set_curterm (term);
  int ret = vidputs (mode, putc);

  set_curterm (prev_term);
  /* mutex down here */
  return ret;
}

/* special hack from ncurses library: there was changes in ncurses library some years ago */
static int
toggled_colors (int c)
{
  if (c < 16)
    {
      int const table[] = { 0, 4, 2, 6, 1, 5, 3, 7, 8, 12, 10, 14, 9, 13, 11, 15 };
      c = table[c];
    }
  return c;
}

static terminal::buffer_type *out_buf = 0;       /**< output buffer */

static int
stream_put (int ch)
{
  return out_buf->sputc (ch);
}

terminal::terminal (terminal::buffer_type &buf, char const *termname)
  : buf_ (buf)
  , attr_ (A_NORMAL)
  , color_ (-1)
{
  term_ = topen (buf_.fd (), termname);
  colors_num_ = ttigetnum (term_, "colors");
  if (colors_num_ < 0)
    colors_num_ = 0;                             /*< black and white*/
}

terminal::~terminal ()
{
  if (term_)
    {
      set_attr ();          /* reset attributes to default */
      reset_colors ();          /* return orig colors */
      tclose (term_);
    }
  reset_shell_mode ();
}

void
terminal::reset_colors ()
{
  if (!colors_num_)
    return;

  /* mutex up here */
  out_buf = &buf_;
  char const *orig_pair_cap = ttigetstr (term_, "op");
  char const *orig_colors_cap = ttigetstr (term_, "oc");
  if (orig_pair_cap)
    tputs (tparm (const_cast<char *> (orig_pair_cap)),
           1, stream_put);
  if (orig_colors_cap)
    tputs (tparm (const_cast<char *> (orig_colors_cap)),
           1, stream_put);
  /* mutex down here */
}

void
terminal::set_attr (attr_t mode)
{
  if (!term_)
    return;

  /* mutex up here */
  out_buf = &buf_;
  tvidputs (term_, (colors_num_ ? (mode | A_COLOR) : mode), stream_put);
  /* mutex down here */
  attr_ = mode;       /* save current mode */
}

void
terminal::set_fg_color (color_t color)
{
  if (!term_ || !colors_num_)
    return;

  /* mutex up here */
  char const *set_a_foreground_cap = ttigetstr (term_, "setaf");
  char const *set_foreground_cap = ttigetstr (term_, "setf");
  out_buf = &buf_;
  if (set_a_foreground_cap)
    tputs (tparm (const_cast<char *> (set_a_foreground_cap), color),
           1, stream_put);
  else if (set_foreground_cap)
    tputs (tparm (const_cast<char *> (set_foreground_cap), toggled_colors (color)),
           1, stream_put);
  /* mutex down here */
  color_ = color;       /* save current color */
}

/* todo read previous attributes before, termattrs() */
void
terminal::set_fg (color_t color, attr_t mode)
{
  set_attr (mode);
  set_fg_color (color);
}

ndk::attr_t
terminal::attr () const
{
  return attr_;
}

ndk::color_t
terminal::color () const
{
  return color_;
}

int
terminal::colors_num () const
{
  return colors_num_;
}
