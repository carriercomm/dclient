/* frame.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#ifndef NDK_FRAME_HH__
#define NDK_FRAME_HH__

#include <ndk/panel.hh>

namespace ndk
{
  /**
   * special version of panel with frame around it's borders
   * you can use it for design widgets
   * Packer class safely work with this frame.
   */
  struct frame
    : panel
  {
    explicit frame (std::string const &title = "", panel *parent = 0);

  protected:
    virtual void draw ();               // draw frame on the panel
    virtual int padding () const;       // drawing area is smaller now

    void set_title (std::string const &title);
    std::string const &title () const;

    bool active () const { return active_; }

  private:
    event::result at_system (const event &);

    std::string title_;
    bool active_;
  };
}

#endif
