#pragma once

#include <string>

namespace data
{
  struct inventory;
}

namespace interface
{
  struct inventory_handler
  {
    virtual void update (data::inventory const &inv, std::string const &title) = 0;
  };
}
