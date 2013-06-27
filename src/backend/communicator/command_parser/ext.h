#include <json/json.h>

void
command_parser::on_ext (std::istream &args)
{
  Json::Reader reader;
  Json::Value ext;
  reader.parse (args, ext, false);
  PARSE_END;

  emit ext (ext);
}
