#include "dclient/render_glyph.h"

#include <ndk/color.hh>
#include <ndk/pen.hh>

#include "adt/array_size.h"

#include <cassert>
#include <utility>

struct color_attr
{
  int color;
  attr_t attr;
};

static color_attr const foreground[] = {
  { COLOR_WHITE,      WA_NORMAL  }, // black      =>  0,
  { COLOR_WHITE,      WA_BOLD    }, // white      =>  1,
  { COLOR_BLUE,       WA_NORMAL  }, // navy       =>  2,
  { COLOR_RED,        WA_NORMAL  }, // red        =>  3,
  { COLOR_RED,        WA_BOLD    }, // orange     =>  4,
  { COLOR_BLUE,       WA_BOLD    }, // blue       =>  5,
  { COLOR_RED,        WA_NORMAL  }, // darkorange =>  6,
  { COLOR_GREEN,      WA_NORMAL  }, // green      =>  7,
  { COLOR_GREEN,      WA_BOLD    }, // lightgreen =>  8,
  { COLOR_BLACK,      WA_BOLD    }, // grey       =>  9,
  { COLOR_YELLOW,     WA_NORMAL  }, // brown      => 10,
  { COLOR_YELLOW,     WA_BOLD    }, // gold       => 11,
  { COLOR_CYAN,       WA_NORMAL  }, // tan        => 12,
  { COLOR_WHITE,      WA_NORMAL  }, // none       => 13,
  {                              }, //            => 14,
  {                              }, //            => 15,
};

static color_attr const background[] = {
  { COLOR_BLACK,      WA_NORMAL  }, // black      =>  0,
  { COLOR_WHITE,      WA_BOLD    }, // white      =>  1,
  { COLOR_BLUE,       WA_NORMAL  }, // navy       =>  2,
  { COLOR_RED,        WA_NORMAL  }, // red        =>  3,
  { COLOR_RED,        WA_BOLD    }, // orange     =>  4,
  { COLOR_BLUE,       WA_BOLD    }, // blue       =>  5,
  { COLOR_RED,        WA_NORMAL  }, // darkorange =>  6,
  { COLOR_GREEN,      WA_NORMAL  }, // green      =>  7,
  { COLOR_GREEN,      WA_BOLD    }, // lightgreen =>  8,
  { COLOR_BLACK,      WA_BOLD    }, // grey       =>  9,
  { COLOR_YELLOW,     WA_NORMAL  }, // brown      => 10,
  { COLOR_YELLOW,     WA_BOLD    }, // gold       => 11,
  { COLOR_CYAN,       WA_NORMAL  }, // tan        => 12,
  { COLOR_BLACK,      WA_NORMAL  }, // none       => 13,
  {                              }, //            => 14,
  {                              }, //            => 15,
};

static ndk::color colors[array_size (foreground) * array_size (background)];

static struct color_initialiser
{
  color_initialiser ()
  {
    static_assert (array_size (foreground) == 16, "invalid size of foreground table");
    static_assert (array_size (background) == 16, "invalid size of background table");
    for (size_t fg = 0; fg < array_size (foreground); fg++)
      for (size_t bg = 0; bg < array_size (background); bg++)
        {
          color_attr fgpair = foreground[fg];
          color_attr bgpair = background[bg];

          colors[fg + bg * array_size (foreground)] = {
            fgpair.color,
            bgpair.color,
            fgpair.attr | bgpair.attr,
          };
        }
  }
} initialiser;


ndk::color
ndi_color (int fg, int bg)
{
  assert (fg < int (array_size (foreground)));
  assert (bg < int (array_size (background)));
  return colors[fg + bg * array_size (foreground)];
}

static void
write (ndk::pen &pen, data::glyph::part const &part, u16 darkness)
{
  pen.write (darkness < 50 ? ndi_color (9, 0) : ndi_color (part.fg, part.bg), part.text);
}

void
write (ndk::pen &pen, data::glyph const &glyph, u16 darkness)
{
  write (pen, glyph[0], darkness);
  write (pen, glyph[1], darkness);
}
