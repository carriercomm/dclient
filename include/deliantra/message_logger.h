#pragma once

#include "interface/command_handler.h"

struct message_logger
  : interface::command_handler
{
  virtual void addme_success ();
  virtual void addspell ();
  //virtual void anim (); // TODO
  virtual void delinv (u32 tag);
  virtual void delitem (std::vector<u32> const &items);
  virtual void drawinfo (int level, std::string const &msg);
  virtual void ext (Json::Value const &ext);
  virtual void fx (std::vector<data::fx> const &faces);
  virtual void item2 (u32 location, std::vector<data::item> const &items);
  virtual void map (data::map const &map);
  virtual void map_scroll (int dx, int dy);
  //virtual void mapinfo (); // TODO
  virtual void msg (int level, std::string const &channel, std::string const &msg);
  virtual void player (u32 tag, u32 weight, u32 face, std::string const &name);
  virtual void query (int flags, std::string const &prompt);
  virtual void replyinfo (std::string const &title, std::vector<std::pair<int, std::string>> const &info);
  //virtual void setup (); // TODO
  virtual void stats (data::statistics const &stats);
  virtual void upditem (u08 flags, u32 location, data::item const &item);
  //virtual void updspell (); // TODO
  virtual void version (Json::Value const &server_version);
};
