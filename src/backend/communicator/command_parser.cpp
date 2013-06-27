#include "deliantra/command_parser.h"

#include <boost/range/adaptor/reversed.hpp>

#include <cassert>
#include <sstream>
#include <stdexcept>

void
command_parser::receive (std::string const &cmd, std::string const &args)
{
  std::istringstream argstream (args);
  if (false) ;
  else if (cmd == "addme_success") on_addme_success (argstream);
  else if (cmd == "addspell"     ) on_addspell      (argstream);
  else if (cmd == "anim"         ) on_anim          (argstream);
  else if (cmd == "delinv"       ) on_delinv        (argstream);
  else if (cmd == "delitem"      ) on_delitem       (argstream);
  else if (cmd == "drawinfo"     ) on_drawinfo      (argstream);
  else if (cmd == "ext"          ) on_ext           (argstream);
  else if (cmd == "fx"           ) on_fx            (argstream);
  else if (cmd == "item2"        ) on_item2         (argstream);
  else if (cmd == "map1a"        ) on_map1a         (argstream);
  else if (cmd == "mapinfo"      ) on_mapinfo       (argstream);
  else if (cmd == "map_scroll"   ) on_map_scroll    (argstream);
  else if (cmd == "msg"          ) on_msg           (argstream);
  else if (cmd == "newmap"       ) on_newmap        (argstream);
  else if (cmd == "player"       ) on_player        (argstream);
  else if (cmd == "query"        ) on_query         (argstream);
  else if (cmd == "replyinfo"    ) on_replyinfo     (argstream);
  else if (cmd == "setup"        ) on_setup         (argstream);
  else if (cmd == "stats"        ) on_stats         (argstream);
  else if (cmd == "upditem"      ) on_upditem       (argstream);
  else if (cmd == "updspell"     ) on_updspell      (argstream);
  else if (cmd == "version"      ) on_version       (argstream);
  else
    {
      LOG () << "unhandled command: " << cmd << " " << args << "\n";
      throw std::runtime_error ("unknown command: `" + cmd + "' args:" + args);
    }
}

void
command_parser::add_command_handler (interface::command_handler &handler)
{
  handlers.push_back (&handler);
}


#include "deliantra/netio.h"

static std::pair<std::string, std::string>
split2 (std::string const &s, char ch)
{
  size_t pos = s.find_first_of (ch);
  return { s.substr (0, pos), s.substr (pos + 1) };
}

#define PARSE_END                       \
  return_unless (args);                 \
  return_unless (args.peek () == EOF)

#define emit for (interface::command_handler *handler : handlers | boost::adaptors::reversed) handler->

#include "command_parser/addme_success.h"
#include "command_parser/addspell.h"
#include "command_parser/anim.h"
#include "command_parser/delinv.h"
#include "command_parser/delitem.h"
#include "command_parser/drawinfo.h"
#include "command_parser/ext.h"
#include "command_parser/fx.h"
#include "command_parser/item2.h"
#include "command_parser/map1a.h"
#include "command_parser/mapinfo.h"
#include "command_parser/map_scroll.h"
#include "command_parser/msg.h"
#include "command_parser/newmap.h"
#include "command_parser/player.h"
#include "command_parser/query.h"
#include "command_parser/replyinfo.h"
#include "command_parser/setup.h"
#include "command_parser/stats.h"
#include "command_parser/upditem.h"
#include "command_parser/updspell.h"
#include "command_parser/version.h"
