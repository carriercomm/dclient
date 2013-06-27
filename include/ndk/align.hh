/* packer.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#pragma once

namespace ndk
{
  struct panel;

  // NDK Aligments
  namespace align
  {
    enum type           // possible aligments of the objects
    {
      left,             // to the left edge
      right,            // to the right edge
      center            // center object
    };

    /**
     * tell us where to move widget to align it's under other widget
     * note: this will return relative coordinate, you may want to convert it into absolute after it
     */
    int pos (int internal_size, int external_size, type align);
  }
}
