void
command_parser::on_delinv (std::istream &args)
{
  u32 tag;
  args >> tag;
  PARSE_END;

  emit delinv (tag);
}
