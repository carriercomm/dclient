#include "deliantra/data/glyph.h"

std::vector<data::glyph> data::glyphs;

void
data::register_glyph (u32 facenum, glyph const &glyph)
{
  if (glyphs.size () <= facenum)
    glyphs.resize (facenum + 1);
  glyphs[facenum] = glyph;
}
