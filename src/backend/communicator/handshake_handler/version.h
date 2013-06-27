#include <json/json.h>

static std::string
join (char const *str, std::map<char const *, int> const &map)
{
  std::ostringstream out;
  for (auto const &pair : map)
    {
      out << pair.first;
      out << str;
      out << pair.second;
      out << str;
    }
  return out.str ();
}

#if _WIN32
static char const *osver () { return "mswin32"; }
#else
# include <sys/utsname.h>

static std::string
osver ()
{
  utsname name;
  uname (&name);
  return name.sysname;
}
#endif

void
handshake_handler::version (Json::Value const &server_version)
{
  Json::Object client_version {
    { "protver",    1 },
    { "client",     PACKAGE },
    { "clientver",  PACKAGE_VERSION },
    { "osver",      osver () },
  };
  send ("version", client_version);

  std::map<char const *, int> const setup = {
    { "itemcmd",    2 },
    { "mapinfocmd", 1 },
    { "extcmd",     2 },
    { "spellmon",   1 },
    { "excmd",      1 },
    { "lzf",        0 },
    { "frag",       1 },
    { "tileset",    2 },
  };
  send ("setup", join (" ", setup) + "mapsize 31x31");
  
  send ("requestinfo", "skill_info");
  send ("requestinfo", "spell_paths");

  {
    Json::Array resource {
      "resource",
      "b",
      "exp_table",
    };
    send ("exti", resource);
  }

  send ("addme");
}
