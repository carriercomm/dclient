#include "deliantra/data/item.h"

void
command_parser::on_item2 (std::istream &args)
{
  u32 location = read_u32 (args);

  std::vector<data::item> items;

  while (args.peek () != EOF)
    {
      data::item item;
      item.tag = read_u32 (args);
      item.flags = read_u32 (args);
      item.weight = read_u32 (args);
      item.face = read_u32 (args);

      std::string names = read_string08 (args);

      item.anim = read_u16 (args);
      item.animspeed = read_u08 (args);
      item.nrof = read_u32 (args);
      item.type = read_u16 (args);

      auto pair = split2 (names, '\0');
      item.singular = pair.first;
      item.plural   = pair.second;

      items.push_back (item);
    }
  PARSE_END;

  emit item2 (location, items);
}
