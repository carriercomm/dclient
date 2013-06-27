#include "deliantra/data/glyph.h"

#include "deliantra/data/types.h"

namespace ndk
{
  struct pen;
  struct color;
}

ndk::color ndi_color (int fg, int bg);

void write (ndk::pen &pen, data::glyph const &glyph, u16 darkness = -1);
