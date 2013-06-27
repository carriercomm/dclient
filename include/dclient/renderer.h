#include "deliantra/interface/command_handler.h"

#include <string>

struct widgets;

struct renderer
  : interface::command_handler
{
  explicit renderer (widgets &ui);
  ~renderer ();

private:
  virtual void drawinfo (int level, std::string const &msg);
  virtual void map (data::map const &map);
  virtual void msg (int level, std::string const &channel, std::string const &msg);
  virtual void stats (data::statistics const &stats);

  widgets &ui;
};
