#include "deliantra/handshake_handler.h"

handshake_handler::handshake_handler (communicator &comm)
  : sender (comm)
{
}

#include "handshake_handler/version.h"
