#pragma once

#include "communicator.h"

#include <boost/lexical_cast.hpp>

struct sender
{
protected:
  sender (communicator &comm)
    : comm (comm)
  {
  }

  template<typename... Args>
  void send (std::string cmd, Args const &...args)
  {
    cmd += ' ';
    concat (cmd, args...);
    comm.send (cmd);
  }

  void send (char const *buf, size_t len)
  {
    comm.send (std::string (buf, len));
  }

private:
  void concat (std::string const &out)
  {
  }

  template<typename T, typename... Args>
  void concat (std::string &out, T const &arg, Args const &...args)
  {
    out += boost::lexical_cast<std::string> (arg);
    concat (out, args...);
  }

  communicator &comm;
};
