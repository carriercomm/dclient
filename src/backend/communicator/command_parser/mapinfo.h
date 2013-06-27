void
command_parser::on_mapinfo (std::istream &args)
{
  LOG ("server") << "mapinfo (" << args.rdbuf () << ")\n";
}
