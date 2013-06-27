/* palette.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#ifndef NDK_XX_PALETTE_HH__
#define NDK_XX_PALETTE_HH__

#include <unordered_map>

#include <ndk/color.hh>
#include <ndk/support.hh>

namespace ndk
{
  /**
   * all possible colors for any NDK application
   * you can redefine some of them in your application
   */
  namespace colors
  {
    enum type
    {
      panel,                                  /**< we fill panel with this color */
      frame1,                                  /**<first color we use to draw frame */
      frame2,                                  /**<second color we use to draw frame */
      label,                                  /**< label */
      button_default,                                  /**< default button view*/
      button_selected,                                  /**< button when it's selected */
      button_focused,                                  /**< button,when it's focused */
      title,                                  /**< color of the title */
      minibutton,                                  /**< minimalistic buttons */
      edit_active,                                  /**< active line edit */
      edit_inactive                                  /**< inactive line edit */
    };
  }

  typedef std::unordered_map<colors::type, color> palette_hash;       /**< hash of the colors, key - type of color*/

  typedef singleton_holder<palette_hash> palette;       /**< general palette for NDK engine */
}

#endif
