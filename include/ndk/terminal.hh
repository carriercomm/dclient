/* terminal.hh
 * This file is part of ncursesxx library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libncursesxx as the file COPYING.
 */
#ifndef NCURSESXX_TERMINAL_HH__
#define NCURSESXX_TERMINAL_HH__

#include <curses.h>
#include <term.h>

#include <ext/stdio_filebuf.h>

namespace ndk
{
  typedef short color_t;
  using ::attr_t;

  /**
   * ncurses terminal object.
   */
  struct terminal
  {
    typedef __gnu_cxx::stdio_filebuf<char> buffer_type;

    // init terminal, open descriptor pointing to file descriptor
    explicit terminal (buffer_type &buf, const char *termname = 0);
    ~terminal ();

    // send control sequences to terminal to set foreground color and attributes
    void set_attr (attr_t mode = A_NORMAL);                     // set terminal attributes
    void set_fg_color (color_t color = COLOR_WHITE);            // set foreground color
    void set_fg (color_t color, attr_t mode = A_NORMAL);        // set foreground color and attributes

    color_t color () const;                                     // return current terminal color
    attr_t  attr () const;                                      // return current terminal attributes

    int colors_num () const;                                    // return number of possible colors

    void reset_colors ();                                       // reset all colors and color pairs

  private:
    buffer_type &buf_;                                          // pointer to output buffer
    TERMINAL *term_;                                            // ncurses terminal object
    attr_t attr_;                                               // current terminal attributes
    int colors_num_;                                            // maximum number of the colors
    color_t color_;                                             // current terminal color
  };
}

#endif
