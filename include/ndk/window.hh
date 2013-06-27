/* window.hh
 * This file is part of ncursesxx library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libncursesxx as the file COPYING.
 */
#pragma once

#include <ndk/support.hh>

#include <curses.h>

#include <memory>

namespace ndk
{

  /**
   * Generic interface to NCurses window, it's only a container with general
   * functions, more powerful interface with subwindows and moving will be
   * provided by panel note: derived windows (and subwindows) are unusable,
   * because their separate single buffer with window, so I use only general
   * windows ().
   */
  struct window
  {
    explicit window ();
    virtual ~window ();

    virtual void resize (int w, int h);

    int width () const;
    int height () const;
    int x () const;
    int y () const;
    int right_x () const { return x () + width (); }
    int bottom_y () const { return y () + height (); }

    /**
     * mark line of the window as needed to update
     * @param num number of the line
     * @param count number of the lines to touch
     */
    void touch_line (int num, int count = 1);
    void commit ();            /**< commit window changes to virtual screen */

    void set_scroll (bool res = false);            /**< scroll window content or not */

    void update () { wrefresh (win ()); }

    WINDOW *win () const { return win_.get (); }

  private:
    std::unique_ptr<WINDOW, int (*)(WINDOW *)> win_;
  };

  /**
   * copy window content from source to destination
   */
  void copy (window &source, window &destination);
}
