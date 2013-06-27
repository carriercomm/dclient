/* label.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#ifndef NDK_XX_LABEL_HH__
#define NDK_XX_LABEL_HH__

#include <string>

#include <ndk/pen.hh>
#include <ndk/panel.hh>

namespace ndk
{
  /**
   * General label object, add to ndk::panel
   * feature to set/get and draw text
   */
  struct label
    : panel
  {
    label (const std::string &str, panel *parent = 0);

    void set_text (const std::string &str);            /**< set label text */
    const std::string &text () const;            /**< get label text */

    virtual void draw ();            /**< draw text in the window, don't made refresh */

  protected:
    void draw (const std::string &str, std::string::size_type begin);            /* draw text on the label */
    virtual void set_color (pen &p);           /** set default color of the label */
    std::string text_;                 /**< label's text */
  };
}

#endif
