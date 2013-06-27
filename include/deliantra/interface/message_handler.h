#pragma once

#include <string>

namespace interface
{
  struct message_handler
  {
    virtual void receive (std::string const &command, std::string const &args) = 0;
  };
}
