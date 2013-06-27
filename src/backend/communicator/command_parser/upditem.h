void
command_parser::on_upditem (std::istream &args)
{
  u08 const flags = read_u08 (args);
  u32 location = 0;

  data::item item { };

  item.tag = read_u32 (args);

  if (flags & UPD_LOCATION) location       = read_u32 (args);

  if (flags & UPD_FLAGS)    item.flags     = read_u32 (args);
  if (flags & UPD_WEIGHT)   item.weight    = read_u32 (args);
  if (flags & UPD_FACE)     item.face      = read_u32 (args);

  if (flags & UPD_NAME)
    {
      std::string names = read_string08 (args);

      auto pair = split2 (names, '\0');
      item.singular = pair.first;
      item.plural   = pair.second;
    }

  if (flags & UPD_ANIM)      item.anim      = read_u16 (args);
  if (flags & UPD_ANIMSPEED) item.animspeed = read_u08 (args);
  if (flags & UPD_NROF)      item.nrof      = read_u32 (args);

  PARSE_END;

  emit upditem (flags, location, item);
}
