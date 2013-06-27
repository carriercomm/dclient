#include <cstring>

#include <iostream>

#include "adt/array_size.h"

#include "deliantra/data/glyph.h"
#include "deliantra/data/map.h"

#include "trenderer.h"

void
renderer::draw_map () const
{
  for (size_t y = 0; y < array_size (win); y++)
    {
      wchar_t buf[array_size (win[y]) + 1];
      memcpy (buf, win[y], sizeof win[y]);
      buf[array_size (win[y])] = L'\0';
      printf ("%ls\n", buf);
    }
}


void
renderer::drawinfo (int level, std::string const &msg)
{
  std::cout << "drawinfo: " << msg << '\n';
}


static u16
select_face (u16 const (&face)[3], int layer)
{
  if (layer == -1)
    return face[2]
           ? face[2]
           : face[1]
             ? face[1]
             : face[0]
             ;
  return face[layer];
}

void
renderer::map (data::map const &map)
{
  static int const layer = -1;

  for (size_t y = 0; y < array_size (win); y++)
    for (size_t x = 0; x < array_size (win[y]); x++)
      win[y][x] = L'_';

  static data::glyph const space  = { { {  0,  0, L'?' }, {  0,  0, L'?' } } };
  static data::glyph const player = { { {  0,  0, L'@' }, {  0,  0, L'@' } } };

  int const playerx = (W - !(W % 2)) / 2;
  int const playery = (H - !(H % 2)) / 2;

  int const offx = (map.w - W) / 2;
  int const offy = (map.h - H) / 2;

  return_unless (!data::glyphs.empty ());
 
  for (int y = 0; y < H; y++)
    {
      int const mapy = y + map.y + offy;
      if (!map.rows.has (mapy))
        continue;
      auto const &row = map.rows[mapy];
      for (int x = 0; x < W; x++)
        {
          int const mapx = x + map.x + offx;
          if (!row.has (mapx))
            continue;
          data::map::cell const &cell = row[mapx];
          u32 const face = select_face (cell.face, layer);

          data::glyph const *glyph;

          int const winy = y * 1;
          int const winx = x * 2;

          if (x == playerx && y == playery)
            glyph = &player;
          else if (face && face < data::glyphs.size ()) // TODO: warn about face >= size
            glyph = &data::glyphs[face];
          else
            glyph = &space;

          return_unless (winy >= 0);
          return_unless (winx >= 0);
          return_unless (winy < int (array_size ( win)));
          return_unless (winx < int (array_size (*win)));

          win[winy][winx + 0] = (*glyph)[0].text;
          win[winy][winx + 1] = (*glyph)[1].text;
        }
    }
}


void
renderer::map_scroll (int dx, int dy)
{
  std::cout << "map_scroll " << dx << ' ' << dy << '\n';
}


void
renderer::msg (int level, std::string const &channel, std::string const &msg)
{
  std::cout << channel << ": " << msg << '\n';
}


void
renderer::newmap ()
{
  std::cout << "newmap\n";
}


void
renderer::stats (data::statistics const &stats)
{
}
