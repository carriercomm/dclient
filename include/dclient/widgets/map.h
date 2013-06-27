#pragma once

#include <ndk/frame.hh>

namespace data
{
  struct map;
}

struct map_widget
  : ndk::frame
{
  explicit map_widget (panel *parent = 0);

  void update (data::map const &map);

private:
  virtual void draw ();

  data::map const *map;
};
