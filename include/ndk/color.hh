#pragma once

#include <curses.h>

namespace ndk
{
  struct color
  {
    color (int fg = COLOR_WHITE, int bg = COLOR_BLACK, attr_t attr = WA_NORMAL)
      : wattr ((attr & ~A_COLOR) | COLOR_PAIR (fg * 8 + bg + 1))
    {
    }

    attr_t attr () const { return wattr;               }
    int    num  () const { return PAIR_NUMBER (wattr); }

  private:
    int wattr;
  };
}
