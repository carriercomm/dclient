/* basic_pen.hh
 * This file is part of ncursesxx library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libncursesxx as the file COPYING.
 */
#pragma once

#include <string>
#include <cstdio>

#include <curses.h>

namespace ndk
{
  struct color;
  struct pad;
  struct window;

  /**
   * pen to draw something inside window,
   */
  struct pen
  {
    static void write (WINDOW *win, char const *str, int len);
    static void write (WINDOW *win, std::string const &str);
    static void write (WINDOW *win, char ch);
    static void write (WINDOW *win, wchar_t ch);
    static void write (WINDOW *win, color c, wchar_t wc);
    static void frame (WINDOW *win);
    static wchar_t const &ltitle   (bool bold = false);
    static wchar_t const &rtitle   (bool bold = false);
    static wchar_t const &ulcorner (bool bold = false);
    static wchar_t const &llcorner (bool bold = false);
    static wchar_t const &urcorner (bool bold = false);
    static wchar_t const &lrcorner (bool bold = false);
    static wchar_t const &h_line   (bool bold = false);
    static wchar_t const &v_line   (bool bold = false);

  public:
    explicit pen (window &win);
    explicit pen (pad &pad);
    bool move (int x, int y) __wur;
    void commit ();
    void getpos (int &x, int &y) const;
    int x () const;
    int y () const;
    void clear ();
    void fill (color col);
    void set_color (color c);
    void frame ();
    void title (std::string const &title);
    void write (std::string const &str);
    void write (char ch);
    void write (wchar_t ch);
    void write (color c, wchar_t ch);

    template<size_t N>
    void write (char const (&str)[N])
    {
      write (win_, str, N - 1);
    }

    template<size_t N, typename... Args>
    void write (char const (&fmt)[N], Args const &...args)
    {
      char buf[BUFSIZ];
      write (win_, buf, snprintf (buf, sizeof buf, fmt, args...));
    }

  private:
    WINDOW *win_;             /**< window object we draw it*/
  };
}
