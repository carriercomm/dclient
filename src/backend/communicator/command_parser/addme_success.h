void
command_parser::on_addme_success (std::istream &args)
{
  std::string arg;
  args >> arg;
  PARSE_END;

  return_unless (arg == "addme_success");

  emit addme_success ();
}
