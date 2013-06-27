#pragma once

#include <ndk/events.hh>
#include <ndk/panel.hh>

#include <menu.h>

#include <memory>
#include <vector>

namespace ndk
{
  struct menu
    : panel
  {
    menu (panel *parent = 0);
    ~menu ();

    void add_item (char const *name, char const *description);

  private:
    virtual void draw ();

    event::result at_keyboard (keyboard const &ev);

    struct item
    {
      item (item const &other) = delete;
      item &operator = (item const &rhs) = delete;

      item ();
      item (char const *name, char const *description);

      item (item &&other);
      item &operator = (item &&rhs);
      ~item ();

      ITEM *item_;
    };

    std::unique_ptr<MENU, int (*)(MENU *)> menu_;
    std::vector<item> items_;
  };
}
