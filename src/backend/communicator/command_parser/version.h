#include <json/json.h>

void
command_parser::on_version (std::istream &args)
{
  Json::Reader reader;
  Json::Value server_version;
  reader.parse (args, server_version, false);
  PARSE_END;

  emit version (server_version);
}
