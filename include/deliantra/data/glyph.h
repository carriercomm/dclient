#pragma once

#include "constants.h"

#include "types.h"

#include <string>
#include <vector>

namespace data
{
  struct glyph
  {
    struct part
    {
      u08 fg;
      u08 bg;
      wchar_t text;
    } data[2];

    part       &operator [] (size_t n)       { return data[n]; }
    part const &operator [] (size_t n) const { return data[n]; }
  };

  void register_glyph (u32 facenum, glyph const &glyph);

  extern std::vector<glyph> glyphs;
}
