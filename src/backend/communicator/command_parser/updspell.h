void
command_parser::on_updspell (std::istream &args)
{
  LOG ("server") << "updspell (" << lexical_cast<std::string> (args.rdbuf ()).length () << ")\n";
}
