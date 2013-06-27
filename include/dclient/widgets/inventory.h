#pragma once

#include "deliantra/interface/inventory_handler.h"

#include "deliantra/data/types.h"

#include <ndk/frame.hh>

namespace data
{
  struct item;
}

namespace ndk
{
  struct pen;
}

struct inventory_widget
  : ndk::frame
  , interface::inventory_handler
{
  explicit inventory_widget (panel *parent = 0);

private:
  int maxinv () const;

  virtual void update (data::inventory const &inv, std::string const &title);

  static bool draw (ndk::pen &pen, int at, int maxinv, int row, data::item const &item);
  template<typename ContainerT>
  void draw (ndk::pen &pen, ContainerT const &items);
  virtual void draw ();
  ndk::event::result at_keyboard (ndk::keyboard const &ev);

  data::inventory const *inv;
  int at;
};
