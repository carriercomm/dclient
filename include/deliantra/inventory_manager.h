#pragma once

#include "data/inventory.h"
#include "data/item.h"
#include "interface/command_handler.h"
#include "interface/inventory_handler.h"

#include <unordered_map>

struct inventory_manager
  : interface::command_handler
{
  inventory_manager ();
  ~inventory_manager ();

  void set_inventory_view (interface::inventory_handler &view);
  void set_container_view (interface::inventory_handler &view);

private:
  void update_container ();
  void sort ();
  void update_view () const;

  void update ();

  data::item *get (u32 tag);

  virtual void delinv (u32 tag);
  virtual void delitem (std::vector<u32> const &items);
  virtual void item2 (u32 location, std::vector<data::item> const &items);
  virtual void player (u32 tag, u32 weight, u32 face, std::string const &name);
  virtual void upditem (u08 flags, u32 location, data::item const &item);

  std::unordered_map<u32, data::inventory> inventories;
  data::item player_item;
  data::item const ground_item;
  data::item const *container_item;

  interface::inventory_handler *inventory_view;
  interface::inventory_handler *container_view;
};
