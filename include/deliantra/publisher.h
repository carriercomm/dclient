#pragma once

#include <algorithm>
#include <vector>

template<typename ViewT>
struct publisher
{
  typedef ViewT view_type;

  void register_view (view_type &view)
  {
    views.push_back (&view);
  }

  void unregister_view (view_type &view)
  {
    auto found = std::find (views.begin (), views.end (), &view);
    if (found != views.end ())
      views.erase (found);
  }

  std::vector<view_type *> views;
};
