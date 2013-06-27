#pragma once

#include "communicator.h"
#include "interface/message_handler.h"
#include "interface/packet_handler.h"

struct client
  : interface::packet_handler
{
  client (communicator &comm, interface::message_handler &msgh);

private:
  virtual void receive (std::string const &msg);

  communicator &comm;
  interface::message_handler &msgh;
};
