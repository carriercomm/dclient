#include "deliantra/message_logger.h"

#include "deliantra/data/fx.h"
#include "deliantra/data/item.h"

#include <json/json.h>

template<typename T, typename C>
static std::vector<T>
vector (C const &c)
{
  return std::vector<T> (c.begin (), c.end ());
}

template<typename T>
static logstream const &
operator , (logstream const &os, T const &v)
{
  return os << ',' << v;
}

template<typename T>
logstream const &
operator << (logstream const &os, std::vector<T> const &v)
{
  os << '[';
  for (auto it = v.begin (), et = v.end (); it != et; ++it)
    {
      os << *it;
      if (it + 1 != et)
        os << ',';
    }
  os << ']';
  return os;
}

static logstream const &
operator << (logstream const &os, data::fx const &fx)
{
  os << "\n  fx{";
  os << fx.type
      , fx.facenum
      , vector<int> (vector<unsigned char> (fx.name))
      ;
  os << '}';
  return os;
}

static logstream const &
operator << (logstream const &os, data::item const &item)
{
  os << "\n  item{";
  os << item.singular
      , item.plural
      , item.tag
      , item.flags
      , item.weight
      , item.face
      , item.nrof
      , item.anim
      , item.type
      , int (item.animspeed)
      ;
  os << '}';
  return os;
}

template<typename K, typename V>
static logstream const &
operator << (logstream const &os, std::pair<K, V> const &pair)
{
  os << "\n  pair{";
  os << pair.first
      , pair.second
      ;
  os << '}';
  return os;
}

void
message_logger::addme_success ()
{
  LOG ("server") << __func__ << "\n";
}

void
message_logger::addspell ()
{
  LOG ("server") << __func__ << "\n";
}

void
message_logger::delinv (u32 tag)
{
  LOG ("server") << __func__ << " (" << tag << ")\n";
}

void
message_logger::delitem (std::vector<u32> const &items)
{
  LOG ("server") << __func__ << " (" << items << ")\n";
}

void
message_logger::drawinfo (int level, std::string const &msg)
{
  auto const &logs = LOG;
  logs ("server") << __func__ << " (";
  logs << level, msg;
  logs << ")\n";
}

void
message_logger::ext (Json::Value const &ext)
{
  LOG ("server") << __func__ << " (" << ext << ")\n";
}

void
message_logger::fx (std::vector<data::fx> const &faces)
{
  LOG ("server") << __func__ << " (" << faces << ")\n";
}

void
message_logger::item2 (u32 location, std::vector<data::item> const &items)
{
  auto const &logs = LOG;
  logs ("server") << __func__ << " (";
  logs << location, items;
  logs << ")\n";
}

void
message_logger::map (data::map const &map)
{
  //LOG ("server") << __func__ << "\n";
}

void
message_logger::map_scroll (int dx, int dy)
{
  auto const &logs = LOG;
  logs ("server") << __func__ << " (";
  logs << dx, dx;
  logs << ")\n";
}

void
message_logger::msg (int level, std::string const &channel, std::string const &msg)
{
  auto const &logs = LOG;
  logs ("server") << __func__ << " (";
  logs << level, channel, msg;
  logs << ")\n";
}

void
message_logger::player (u32 tag, u32 weight, u32 face, std::string const &name)
{
  auto const &logs = LOG;
  logs ("server") << __func__ << " (";
  logs << tag, weight, face, name;
  logs << ")\n";
}

void
message_logger::replyinfo (std::string const &title, std::vector<std::pair<int, std::string>> const &info)
{
  auto const &logs = LOG;
  logs ("server") << __func__ << " (";
  logs << title, info;
  logs << ")\n";
}

void
message_logger::query (int flags, std::string const &prompt)
{
  auto const &logs = LOG;
  logs ("server") << __func__ << " (";
  logs << flags, prompt;
  logs << ")\n";
}

void
message_logger::stats (data::statistics const &stats)
{
  LOG ("server") << __func__ << "\n";
}

void
message_logger::upditem (u08 flags, u32 location, data::item const &item)
{
  auto const &logs = LOG;
  logs ("server") << __func__ << " (";
  logs << int (flags), location, item;
  logs << ")\n";
}

void
message_logger::version (Json::Value const &server_version)
{
  LOG ("server") << __func__ << " (" << server_version << ")\n";
}
