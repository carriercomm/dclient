void
command_parser::on_setup (std::istream &args)
{
  setup.extmap = true;
  LOG ("server") << "setup (" << args.rdbuf () << ")\n";
}
