#include "interface/command_handler.h"
#include "interface/message_handler.h"

#include "data/map.h"
#include "data/setup.h"
#include "data/statistics.h"

#include <fstream>

struct command_parser
  : interface::message_handler
{
  command_parser ()
    : stats ()
    , map ()
    , setup ()
  {
  }

  void add_command_handler (interface::command_handler &handler);

private:
  void receive (std::string const &cmd, std::string const &args);

  void on_addme_success (std::istream &args);
  void on_addspell      (std::istream &args);
  void on_anim          (std::istream &args);
  void on_delinv        (std::istream &args);
  void on_delitem       (std::istream &args);
  void on_drawinfo      (std::istream &args);
  void on_ext           (std::istream &args);
  void on_fx            (std::istream &args);
  void on_item2         (std::istream &args);
  void on_map1a         (std::istream &args);
  void on_mapinfo       (std::istream &args);
  void on_map_scroll    (std::istream &args);
  void on_msg           (std::istream &args);
  void on_newmap        (std::istream &args);
  void on_player        (std::istream &args);
  void on_query         (std::istream &args);
  void on_replyinfo     (std::istream &args);
  void on_setup         (std::istream &args);
  void on_stats         (std::istream &args);
  void on_upditem       (std::istream &args);
  void on_updspell      (std::istream &args);
  void on_version       (std::istream &args);

  std::vector<interface::command_handler *> handlers;

  data::statistics stats;
  data::map map;
  data::setup setup;
};
