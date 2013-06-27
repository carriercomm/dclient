/* application.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#pragma once

#include <ndk/application.hh>

#include <ndk/support.hh>
#include <ndk/transport.hh>

namespace ndk
{
  struct panel;

  /**
   * setup default NDK engine settings, setup palete with default colors
   * application also a head of all Event Transport subsystem
   */
  struct application
  {
    application ();
    ~application ();

    void init_palette ();

    int width () const;         /**< width of the screen */
    int height () const;        /**< height of the screen */
    void cursor_on ();          /**< turn on cursor */
    void cursor_off ();         /**< turn off cursor */

    engine *events ();
    void update ();
    void process (int ch);
    void in_focus (panel &);

    bool has_colors () const { return has_colors_; }

    static void enable_keypad (WINDOW *win);      /* initialise advanced keycodes for this window */

  private:
    SCREEN *screen_;            /**< main ncurses screen */
    engine events_;             /**< entry point to event's roadway */
    bool has_colors_;
  };

  extern application *app;
}
