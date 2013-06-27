#pragma once

#include <ndk/frame.hh>

namespace data
{
  struct statistics;
}

struct stats_widget
  : ndk::frame
{
  explicit stats_widget (panel *parent = 0);

  void update (data::statistics const &stats);

private:
  virtual void draw ();

  data::statistics const *stats;
};
