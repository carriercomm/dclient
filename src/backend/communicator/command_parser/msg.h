#include <json/json.h>

void
command_parser::on_msg (std::istream &args)
{
  if (args.peek () == '[')
    {
      Json::Reader reader;
      Json::Value msg;
      if (!reader.parse (args, msg, false))
        throw std::runtime_error ("error parsing input: " + reader.getFormatedErrorMessages ());
      PARSE_END;

      assert (msg.type () == Json::arrayValue);
      emit msg (msg[0u].asUInt (), msg[1u].asString (), msg[2u].asString ());
    }
  else
    {
      int level;
      std::string channel;
      std::string msg;

      args >> level >> channel;
      while (args.peek () == ' ')
        args.ignore ();
      getline (args, msg);
      PARSE_END;

      emit msg (level, channel, msg);
    }
}
