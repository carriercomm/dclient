#include "deliantra/data/map.h"

#include <string>

using data::map;

void
map::clear ()
{
  x = 0;
  y = 0;
  for (auto &row : rows)
    for (auto &cell : row)
      cell.clear_all ();
}

void
map::ensure (int x, int y)
{
  rows.ensure (y);
  rows.at (y).ensure (x);
}

void
map::blank (int x0, int y0, int w, int h)
{
  return;
  for (int y = y0; y < y0 + h; y++)
    if (y >= 0)
      {
        if (y >= rows.size ())
          break;

        rows.at (y);
      }
}
