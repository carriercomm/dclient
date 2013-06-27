void
command_parser::on_query (std::istream &args)
{
  int flags;
  args >> flags;

  std::string prompt;
  args.ignore (1); // space between flags and message
  prompt.reserve (args.tellg ());
  copy (std::istreambuf_iterator<char> (args), std::istreambuf_iterator<char> (), back_inserter (prompt));

  emit query (flags, prompt);
}
