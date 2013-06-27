void
command_parser::on_newmap (std::istream &args)
{
  map.clear ();

  emit newmap ();
  emit map (map);
}
