#pragma once

#include "deliantra/interface/command_handler.h"
#include "deliantra/sender.h"

struct widgets;

struct query_handler
  : interface::command_handler
  , sender
{
  query_handler ( communicator &comm
                , widgets &ui
                , std::string const &username
                , std::string const &password
                );

private:
  virtual void query (int flags, std::string const &prompt);

  widgets &ui;
  std::string const username;
  std::string const password;
};
