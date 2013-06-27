void
command_parser::on_replyinfo (std::istream &args)
{
  std::string title;
  std::getline (args, title);

  std::vector<std::pair<int, std::string>> info;
  while (args.peek () != EOF)
    {
      int key;
      args >> key;
      {
        char colon = args.get ();
        assert (colon == ':');
      }
      std::string value;
      getline (args, value);

      info.emplace_back (key, move (value));

      assert (args);
    }
  PARSE_END;

  emit replyinfo (title, info);
}
