void
command_parser::on_player (std::istream &args)
{
  u32 tag = read_u32 (args);
  u32 weight = read_u32 (args);
  u32 face = read_u32 (args);
  std::string name = read_string08 (args);
  PARSE_END;

  emit player (tag, weight, face, name);
}
