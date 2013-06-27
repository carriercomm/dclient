#pragma once

#include "adt/rebased_vector.h"

#include "counters.h"
#include "types.h"

#include <iosfwd>
#include <vector>

namespace data
{
  struct map
  {
    struct cell
    {
      u32 player;
      u16 face[3];
      u16 darkness;
      u08 stat_width;
      u08 stat_hp;
      u08 flags;


      cell ()
        : player (0)
      {
        ++cell_count;
        clear_all ();
      }

      ~cell ()
      {
        --cell_count;
      }

      void clear ()
      {
        if (player)
          face[2] = 0;
        player = 0;
        darkness = 0;
        stat_width = 0;
        stat_hp = 0;
        flags = 0;
      }

      void clear_all ()
      {
        clear ();
        face[0] = face[1] = face[2] = 0;
      }
    };

    struct row
      : rebased_vector<cell>
    {
      int c0, c1;
    };

    map ()
      : x (0)
      , y (0)
      , w (31)
      , h (31)
    {
    }

    cell &operator () (int x, int y)
    {
      ensure (x, y);
      return rows.at (y).at (x);
    }

    void clear ();
    void ensure (int x, int y);
    void blank (int x0, int y0, int w, int h);

    rebased_vector<row> rows;

    int x, y;
    int w, h;
  };
}
