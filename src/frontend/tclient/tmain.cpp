#include "tinput_handler.h"
#include "tquery_handler.h"
#include "trenderer.h"

#include "deliantra/client.h"
#include "deliantra/command_parser.h"
#include "deliantra/communicator.h"
#include "deliantra/handshake_handler.h"
#include "deliantra/inventory_manager.h"
#include "deliantra/message_logger.h"

#include "deliantra/data/item.h"

#include <ndk/application.hh>

#include <csignal>
#include <cstring>
#include <iomanip>
#include <memory>

static void
start_game ( std::string const &server
           , std::string const &port
           , std::string const &user
           , std::string const &pass
           )
{
  if (!ev_default_loop (EVBACKEND_SELECT))
    throw std::runtime_error ("unable to initialise event loop");

  // deliantra
  communicator comm (server, port);
  command_parser cmdp;
  client cli (comm, cmdp);
  handshake_handler hh (comm);
  message_logger logger;
  inventory_manager invman;

  cmdp.add_command_handler (hh);
  cmdp.add_command_handler (invman);
  cmdp.add_command_handler (logger);

  // link deliantra stuff together
  comm.set_packet_handler (cli);

  // tclient
  input_handler ih (comm);
  query_handler qh (comm, user, pass);
  renderer rend;

  // link tclient stuff in
  cmdp.add_command_handler (qh);
  cmdp.add_command_handler (rend);

  ih.rend = &rend;

  // run the event loop
  ev_run (EV_DEFAULT_UC_ 0);
}

#include "../main.h"
