#pragma once

#include <string>

namespace interface
{
  struct packet_handler
  {
    virtual void receive (std::string const &msg) = 0;
  };
}
