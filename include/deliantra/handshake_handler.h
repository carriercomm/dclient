#pragma once

#include "interface/command_handler.h"
#include "sender.h"

struct handshake_handler
  : interface::command_handler
  , sender
{
  handshake_handler (communicator &comm);

private:
  virtual void version (Json::Value const &server_version);
};
