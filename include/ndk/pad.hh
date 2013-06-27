/* pad.hh
 * This file is part of ncursesxx library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libncursesxx as the file COPYING.
 */
#ifndef NCURSESXX_PAD_HH__
#define NCURSESXX_PAD_HH__

#include <ndk/support.hh>

#include <curses.h>

#include <memory>

namespace ndk
{
  class window;

  /**
   * This is an abstraction of NCurses Pad feature - big window, which content you
   * can se via small viewport window. You can use it as an internal big text buffer
   * for the widgets
   */
  struct pad
  {
    explicit pad ();
    virtual ~pad ();

    virtual void resize (int w, int h);

    int width () const;
    int height () const;

    void draw (window &viewport, int x = 0, int y = 0);

    WINDOW *win () const { return pad_.get (); }

  private:
    std::unique_ptr<WINDOW, int (*)(WINDOW *)> pad_;             /**< pad window */
  };
}

#endif
