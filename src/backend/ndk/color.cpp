/* color.cc
 * This file is part of ncursesxx library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libncursesxx as the file COPYING.
 */
#include <ndk/color.hh>

#include <ndk/application.hh>

#include <bitset>

using ndk::color;

static std::bitset<64> initialised;

color::color (short fg, short bg, attr_t attr)
  : attr_ (attr)
  , num_ (fg * 8 + bg + 1)
{
  if (app->has_colors ())
    if (!initialised[num_ - 1])
      {
        initialised[num_ - 1] = true;
        return_unless (init_pair (num_, fg, bg) == OK);
      }
}
