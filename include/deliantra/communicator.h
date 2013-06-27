#pragma once

#include "interface/packet_handler.h"
#include "data/types.h"

#include <vector>

#include <ev++.h>

// Handles the low level sending and receiving commands.
struct communicator
{
  /// Send command to the server.
  void send (std::string const &command);

  communicator (std::string const &server, std::string const &port);

  void set_packet_handler (interface::packet_handler &handler);

private:
  enum response_state
  {
    LENGTH,
    COMMAND
  };

  void read_cb (ev::io &w, int revents);

  interface::packet_handler *on;
  ev::io sock;

  size_t response_len;
  unsigned char response[65538];
};
