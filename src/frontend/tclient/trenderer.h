#include "deliantra/interface/command_handler.h"

#include <string>

struct widgets;

struct renderer
  : interface::command_handler
{
  void draw_map () const;

private:
  virtual void drawinfo (int level, std::string const &msg);
  virtual void map (data::map const &map);
  virtual void map_scroll (int dx, int dy);
  virtual void msg (int level, std::string const &channel, std::string const &msg);
  virtual void newmap ();
  virtual void stats (data::statistics const &stats);

  static int const W = 27;
  static int const H = 21;

  wchar_t win[H][W * 2];
};
