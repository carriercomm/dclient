#pragma once

#include "types.h"

#include <string>

namespace data
{
  struct item
  {
    item (std::string const &singular = "", std::string const &plural = "")
      : singular (singular)
      , plural (plural)
      , tag (0)
      , flags (0)
      , weight (0)
      , face (0)
      , nrof (0)
      , anim (0)
      , type (0)
      , animspeed (0)
    {
    }

    std::string singular;
    std::string plural;
    u32 tag;
    u32 flags;
    u32 weight;
    u32 face;
    u32 nrof;
    u16 anim;
    u16 type;
    u08 animspeed;
  };
}
