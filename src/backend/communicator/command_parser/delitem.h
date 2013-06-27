void
command_parser::on_delitem (std::istream &args)
{
  std::vector<u32> items;
  while (args.peek () != EOF)
    {
      items.push_back (read_u32 (args));
      assert (args);
    }
  PARSE_END;

  emit delitem (items);
}
