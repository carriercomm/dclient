#include "deliantra/data/fx.h"
#include "deliantra/data/glyph.h"

static bool
read_glyph_part (char const *&data, size_t &length, data::glyph::part &part)
{
  return_val_unless (length > 2, false);

  part.fg = *data++; length--;
  part.bg = *data++; length--;

  int len;
  return_val_unless ((len = mbtowc (&part.text, data, length)) != -1, false);

  data   += len;
  length -= len;

  return true;
}

static void
read_glyph (u32 facenum, std::string name)
{
  data::glyph glyph;
  if (!name.empty ())
    {
      char const *data = name.data ();
      size_t length = name.length ();
      return_unless (read_glyph_part (data, length, glyph[0]));
      return_unless (read_glyph_part (data, length, glyph[1]));
      return_unless (length == 0);
    }
  data::register_glyph (facenum, glyph);
}

void
command_parser::on_fx (std::istream &args)
{
  std::vector<data::fx> faces;

  u32 type = 0;
  while (args.peek () != EOF)
    {
      u32 facenum = read_ber32 (args);
      std::string name = read_string08 (args);

      if (facenum)
        {
          read_glyph (facenum, name);
          faces.emplace_back (data::fx { type, facenum, move (name) });
        }
      else if (name.length () == 1)
        type = name[0];
    }
  PARSE_END;

  emit fx (faces);
}
