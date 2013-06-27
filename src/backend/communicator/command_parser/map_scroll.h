void
command_parser::on_map_scroll (std::istream &args)
{
  int dx, dy;
  args >> dx >> dy;
  PARSE_END;

  if (dx > 0)
    map.blank (map.x             , map.y             ,  dx  , map.h);
  else if (dx < 0)
    map.blank (map.x + map.w + dx, map.y             , -dx  , map.h);

  if (dy > 0)
    map.blank (map.x             , map.y             , map.w,  dy);
  else if (dy < 0)
    map.blank (map.x             , map.y + map.h + dy, map.w, -dy);

  map.x += dx;
  map.y += dy;

  emit map_scroll (dx, dy);
}
