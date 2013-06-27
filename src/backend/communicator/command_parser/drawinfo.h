void
command_parser::on_drawinfo (std::istream &args)
{
  int level;
  std::string msg;

  args >> level;
  while (args.peek () == ' ')
    args.ignore ();
  getline (args, msg);
  PARSE_END;

  emit drawinfo (level, msg);
}
