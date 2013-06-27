#include "ndk/keymap.hh"
#include "ndk/events.hh"

#include "adt/string.h"

#include <unordered_map>
#include <stdexcept>

using ndk::key;

typedef std::unordered_map<std::string, key> escape_map;

static escape_map const urxvt_keymap = {
  { "\033[A", key::up },
  { "\033[B", key::down },
  { "\033[C", key::right },
  { "\033[D", key::left },

  { "\033[a", key::shift_up },
  { "\033[b", key::shift_down },
  { "\033[c", key::shift_right },
  { "\033[d", key::shift_left },

  { "\033Oa", key::ctrl_up },
  { "\033Ob", key::ctrl_down },
  { "\033Oc", key::ctrl_right },
  { "\033Od", key::ctrl_left },

  { "\033[3~", key::dc },

  { "\033[5~", key::pg_up },
  { "\033[6~", key::pg_dn },
  { "\033[7~", key::home },
  { "\033[8~", key::end },

  { "\033[11~", key::F1 },
  { "\033[12~", key::F2 },
  { "\033[13~", key::F3 },
  { "\033[14~", key::F4 },
  { "\033[15~", key::F5 },
  { "\033[17~", key::F6 },
  { "\033[18~", key::F7 },
  { "\033[19~", key::F8 },
  { "\033[20~", key::F9 },
  { "\033[21~", key::F10 },
  { "\033[23~", key::F11 },
  { "\033[24~", key::F12 },

  { "\033Oq", key::shift_end },
  { "\033Or", key::shift_down },
  { "\033Os", key::shift_pg_dn },
  { "\033Ot", key::shift_left },
  { "\033Ov", key::shift_right },
  { "\033Ow", key::shift_home },
  { "\033Ox", key::shift_up },
  { "\033Oy", key::shift_pg_up },

  { "\033[5$", key::shift_pg_up },
  { "\033[6$", key::shift_pg_dn },
  { "\033[7$", key::shift_home },
  { "\033[8$", key::shift_end },

  { "\033[5^", key::ctrl_pg_up },
  { "\033[6^", key::ctrl_pg_dn },
  { "\033[7^", key::ctrl_home },
  { "\033[8^", key::ctrl_end },
};

static escape_map const xterm_keymap = {
  { "\033[A", key::up },
  { "\033[B", key::down },
  { "\033[C", key::right },
  { "\033[D", key::left },

  { "\033OA", key::up },
  { "\033OB", key::down },
  { "\033OC", key::right },
  { "\033OD", key::left },

  { "\033[1;2A", key::shift_up },
  { "\033[1;2B", key::shift_down },
  { "\033[1;2C", key::shift_right },
  { "\033[1;2D", key::shift_left },

  { "\033[1;5A", key::ctrl_up },
  { "\033[1;5B", key::ctrl_down },
  { "\033[1;5C", key::ctrl_right },
  { "\033[1;5D", key::ctrl_left },

  { "\033[3~", key::dc },

  { "\033[5~", key::pg_up },
  { "\033[6~", key::pg_dn },
  { "\033[H", key::home },
  { "\033[F", key::end },

  { "\033OE", key::numpad_center },

  { "\033OP", key::F1 },
  { "\033OQ", key::F2 },
  { "\033OR", key::F3 },
  { "\033OS", key::F4 },
  { "\033[15~", key::F5 },
  { "\033[17~", key::F6 },
  { "\033[18~", key::F7 },
  { "\033[19~", key::F8 },
  { "\033[20~", key::F9 },
  { "\033[21~", key::F10 },
  { "\033[23~", key::F11 },
  { "\033[24~", key::F12 },

  { "\033Oq", key::shift_end },
  { "\033Or", key::shift_down },
  { "\033Os", key::shift_pg_dn },
  { "\033Ot", key::shift_left },
  { "\033Ov", key::shift_right },
  { "\033Ow", key::shift_home },
  { "\033Ox", key::shift_up },
  { "\033Oy", key::shift_pg_up },

  { "\033[5;2~", key::shift_pg_up },
  { "\033[6;2~", key::shift_pg_dn },
  { "\033[1;2H", key::shift_home },
  { "\033[1;2F", key::shift_end },

  { "\033[5;5~", key::ctrl_pg_up },
  { "\033[6;5~", key::ctrl_pg_dn },
  { "\033[1;5H", key::ctrl_home },
  { "\033[1;5F", key::ctrl_end },
};

static escape_map const fallback_keymap = xterm_keymap;

#include <fstream>
#include <iomanip>
static std::ofstream logs ("curses.log");

static escape_map const &
select_keymap ()
{
  if (char const *const term = getenv ("TERM"))
    {
      logs << "TERM=" << term << std::endl;
      if (!strncmp (term, "rxvt"))
        return urxvt_keymap;

      if (!strncmp (term, "xterm"))
        return xterm_keymap;
    }
  else
    logs << "TERM unknown" << std::endl;

  return fallback_keymap;
}

static escape_map const &keymap = select_keymap ();


static bool
is_terminator (int const ch)
{
  switch (ch)
    {
    case '^':
    case '~':
    case '$':
    case 'a' ... 'z':
    case 'A' ... 'Z':
      return true;
    default:
      return false;
    }
}

static std::string
escape (std::string const &s)
{
  std::string res;
  res.reserve (s.size () + 2 * 4);

  for (char ch : s)
#if 0
    if (isprint (ch))
      res += s;
    else
#endif
      {
        char buf[5];
        snprintf (buf, sizeof buf, "\\%03o", ch);
        res += buf;
      }

  return res;
}

bool
ndk::has_keymap ()
{
  return &keymap != &fallback_keymap;
}

ndk::key
ndk::parsekey (std::string &esc, int const ch)
{
  key code = key::unknown;

  logs << "chr: \\" << std::oct << std::setfill ('0') << std::setw (3) << ch << std::dec;
#if 0
  logs << "\nERR: \\" << std::oct << std::setfill ('0') << std::setw (3) << ERR << std::dec;
  logs << "\nchr == ERR: " << std::boolalpha << (ch == ERR);
  logs.flush ();
#endif

  if (ch == '\033' || !esc.empty ())
    {
      code = key::intermediate;
      esc += ch;
      logs << ", esc: " << escape (esc);
      if (!strncmp (esc, "\033[M"))
        {
          if (esc.length () == 6)
            code = key::mouse;
        }
      else if (ch != '[' && ch != 'O' && is_terminator (ch))
        {
          escape_map::const_iterator found = keymap.find (esc);
          if (found != keymap.end ())
            {
              code = found->second;
              logs << "\ngot: " << std::oct << std::setfill ('0') << std::setw (3) << int (code) << std::dec;
            }
          else
            {
              code = key::unknown;
              logs << "\nunknown escape sequence: " << escape (esc);
              //throw std::runtime_error ("unknown escape sequence: " + escape (esc));
            }
          // finished
          esc.clear ();
        }
    }
  else if (ch == ERR)
    code = key::error;
  else if (ch == '\177') // XXX: special case: are there more?
    code = key::backspace;
  else
    code = key (ch);

  logs << std::endl;

  return code;
}

ndk::mouse
ndk::getmouse (std::string &esc)
{
  MEVENT mev { };
  if (is_keypad (stdscr))
    {
      return_val_unless (getmouse (&mev) == OK, { mev });
    }
  else
    {
      unsigned char const button = esc[3];
      unsigned char const x      = esc[4];
      unsigned char const y      = esc[5];

      mev.x = x - 33;
      mev.y = y - 33;

      mev.bstate |= (button == 32) * BUTTON1_CLICKED;
      mev.bstate |= (button == 33) * BUTTON2_CLICKED;
      mev.bstate |= (button == 34) * BUTTON3_CLICKED;

      esc.clear ();
    }

  return { mev };
}
