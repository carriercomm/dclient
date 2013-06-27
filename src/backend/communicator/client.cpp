#include "deliantra/client.h"

client::client (communicator &comm, interface::message_handler &msgh)
  : comm (comm)
  , msgh (msgh)
{
}

void
client::receive (std::string const &msg)
{
  size_t space = msg.find_first_of (' ');
  msgh.receive (msg.substr (0, space), msg.substr (space + 1));
}
