#include "deliantra/inventory_manager.h"
#include "deliantra/data/constants.h"

#include "adt/array.h"

#include <algorithm>
#include <cassert>

static bool
type_name_compare (data::item const &a, data::item const &b)
{
  return a.type != b.type ? a.type < b.type : a.singular < b.singular;
}


inventory_manager::inventory_manager ()
  : player_item { }
  , ground_item { "Floor" }
  , container_item (&ground_item)
  , inventory_view (nullptr)
  , container_view (nullptr)
{
}

inventory_manager::~inventory_manager ()
{
}

void
inventory_manager::set_inventory_view (interface::inventory_handler &view)
{
  inventory_view = &view;
}

void
inventory_manager::set_container_view (interface::inventory_handler &view)
{
  container_view = &view;
}


void
inventory_manager::update_container ()
{
  container_item = &ground_item;
  for (auto &pair : inventories)
    for (data::item const &item : pair.second)
      if (item.flags & F_OPEN)
        {
          assert (container_item == &ground_item);
          container_item = &item;
#if NDEBUG
          break;
#endif
        }
}

void
inventory_manager::sort ()
{
  for (u32 tag : array (player_item.tag, container_item->tag))
    if (tag)
      {
        data::inventory &inv = inventories[tag];
        stable_sort (inv.begin (), inv.end (), type_name_compare);
      }
}

void
inventory_manager::update_view () const
{
  for (auto pair : array (std::make_pair (&player_item, inventory_view),
                             std::make_pair (container_item, container_view)))
    {
      if (!pair.second)
        continue;
      auto found = inventories.find (pair.first->tag);
      if (found != inventories.end ())
        pair.second->update (found->second, pair.first->singular);
    }
}

void
inventory_manager::update ()
{
  update_container ();
  sort ();
  update_view ();
}


data::item *
inventory_manager::get (u32 tag)
{
  if (tag == player_item.tag)
    return &player_item;
  for (auto &pair : inventories)
    for (data::item &item : pair.second)
      if (item.tag == tag)
        return &item;
  return NULL;
}

void
inventory_manager::delinv (u32 tag)
{
  inventories.erase (tag);
}

void
inventory_manager::delitem (std::vector<u32> const &itemtags)
{
  assert (player_item.tag != 0);

  for (auto &pair : inventories)
    for (u32 tag : itemtags)
      {
        auto found = find_if (
          pair.second.begin (),
          pair.second.end (),
          [tag] (data::item const &item) {
            return item.tag == tag;
          }
        );

        if (found != pair.second.end ())
          pair.second.erase (found);
      }

  update ();
}

static data::item &
merge (data::inventory &inv, data::item const &item)
{
  for (data::item &inv_item : inv)
    if (inv_item.tag == item.tag)
      return inv_item = item;
  inv.push_back (item);
  return inv.back ();
}

void
inventory_manager::item2 (u32 location, std::vector<data::item> const &items)
{
  assert (player_item.tag != 0);

  data::inventory &inv = inventories[location];
  for (data::item const &item : items)
    merge (inv, item);

  update ();
}

void
inventory_manager::player (u32 tag, u32 weight, u32 face, std::string const &name)
{
  player_item.tag      = tag;
  player_item.weight   = weight;
  player_item.face     = face;
  player_item.singular = name;
}

void
inventory_manager::upditem (u08 flags, u32 location, data::item const &item)
{
  data::item *current = get (item.tag);

  return_unless (current != NULL);

  if (flags & UPD_LOCATION)
    {
      data::item save = *current;
      delitem ({ item.tag });
      current = &merge (inventories[location], save);
    }

  if (flags & UPD_ANIM)      current->anim      = item.anim;
  if (flags & UPD_ANIMSPEED) current->animspeed = item.animspeed;
  if (flags & UPD_FACE)      current->face      = item.face;
  if (flags & UPD_FLAGS)     current->flags     = item.flags;
  if (flags & UPD_NROF)      current->nrof      = item.nrof;
  if (flags & UPD_WEIGHT)    current->weight    = item.weight;

  if (flags & UPD_NAME)
    {
      current->singular = item.singular;
      current->plural   = item.plural;
    }

  update ();
}
