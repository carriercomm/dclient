#include "dclient/widgets/map.h"
#include "dclient/render_glyph.h"

#include "deliantra/data/glyph.h"
#include "deliantra/data/map.h"

#include <ndk/application.hh>
#include <ndk/color.hh>
#include <ndk/pen.hh>

#define HIGHLIGHT_PLAYER 0

map_widget::map_widget (ndk::panel *parent)
  : frame ("", parent)
  , map (nullptr)
{
}

void
map_widget::update (data::map const &map)
{
  this->map = &map;
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
map_widget::draw ()
{
  frame::draw ();

  if (!map)
    return;

  static int const layer = -1;

  int const W = this->width  () / 2;
  int const H = this->height ();
  ndk::pen pen (*this);

  static data::glyph const space  = { { {  0,  0, L' ' }, {  0,  0, L' ' } } };
#if HIGHLIGHT_PLAYER
  static data::glyph const player = { { {  0,  0, L'@' }, { 13, 13, L'@' } } };
#endif

#if HIGHLIGHT_PLAYER
  int const playerx = (W - !(W % 2)) / 2;
  int const playery = (H - !(H % 2)) / 2;
#endif

  int const offx = (map->w - W) / 2;
  int const offy = (map->h - H) / 2;

  return_unless (!data::glyphs.empty ());
 
  for (int y = 1; y < H - 1; y++)
    {
      int const mapy = y + map->y + offy;
      if (!map->rows.has (mapy))
        continue;
      auto const &row = map->rows[mapy];
      for (int x = 1; x < W - 1; x++)
        {
          int const mapx = x + map->x + offx;
          if (!row.has (mapx))
            continue;
          data::map::cell const &cell = row[mapx];
          u32 const face = select_face (cell.face, layer);

          data::glyph const *glyph;

          int const winy = y * 1;
          int const winx = x * 2;

#if HIGHLIGHT_PLAYER
          if (x == playerx && y == playery)
            glyph = &player;
#else
          if (0)
            ;
#endif
          else if (face && face < data::glyphs.size ()) // TODO: warn about face >= size
            glyph = &data::glyphs[face];
          else
            glyph = &space;

          return_unless (winy >= 0);
          return_unless (winx >= 0);
          return_unless (winy < H);
          return_unless (winx < W * 2);

          return_unless (pen.move (winx, winy));
          write (pen, *glyph, cell.darkness);
        }
    }

#if HIGHLIGHT_PLAYER
  // 19:15 < n0nsense> pippijn: kannst du nethack-mäßig noch den cursor auf den player setzen?
  if (active ())
    {
      pen.move (playerx, playery);
      ndk::app->cursor_on ();
    }
  else
    ndk::app->cursor_off ();
#endif

  if (getenv ("DCLIENT_ONESHOT"))
    {
      delete ndk::app;
      exit (0);
    }

  pen.set_color ({ }); // reset
}
