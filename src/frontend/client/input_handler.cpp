#include "deliantra/sender.h"

#include "dclient/input_handler.h"
#include "dclient/render_glyph.h"
#include "dclient/widgets.h"
#include "dclient/widgets/inventory.h"
#include "dclient/widgets/map.h"
#include "dclient/widgets/query.h"
#include "dclient/widgets/stats.h"

#include <ndk/application.hh>
#include <ndk/color.hh>

#include <curses.h>

#include <ev.h>

typedef unsigned long flags_t;

enum class flags
  : flags_t
{
  none,
  firing        = 1 << 0,
  running       = 1 << 1,

  COUNT
};

static flags &
operator += (flags &lhs, flags rhs)
{
  return lhs = flags (flags_t (lhs) | flags_t (rhs));
}

static flags &
operator -= (flags &lhs, flags rhs)
{
  return lhs = flags (flags_t (lhs) & ~flags_t (rhs));
}

static bool
operator & (flags lhs, flags rhs)
{
  return flags_t (lhs) & flags_t (rhs);
}


enum class action
{
  none,

  north,
  northeast,
  east,
  southeast,
  south,
  southwest,
  west,
  northwest,

  run,
  fire,
  stay_fire,

  quit,
  redraw,
  resize,
  maximise,
  input_mode,

  apply,
  take,

  repeat,

  COUNT
};


struct input_handler::pimpl
  : sender
{
  pimpl (communicator &comm, widgets &ui);

  void perform (ndk::panel *sender, action cmd, action arg = action::none);

  ndk::event::result on_kbd_common (ndk::keyboard const &ev);
  ndk::event::result on_kbd_map (ndk::keyboard const &ev);

  void on_command (std::string const &cmd);
  void stdin_read_cb (ev::io &w, int revents);

  widgets &ui;
  std::string esc;
  std::string last_cmd;
  ev::io stdin_watcher;
  flags state;
};

input_handler::pimpl::pimpl (communicator &comm, widgets &ui)
  : sender (comm)
  , ui (ui)
  , state (flags::none)
{
  auto kbd_common = ndk::slot (&pimpl::on_kbd_common, this);

  ui.map->events ()->on_keyboard (kbd_common);
  ui.info->events ()->on_keyboard (kbd_common);
  ui.stats->events ()->on_keyboard (kbd_common);
  ui.inventory->events ()->on_keyboard (kbd_common);
  ui.container->events ()->on_keyboard (kbd_common);
  ui.invmenu->events ()->on_keyboard (kbd_common);
  ui.query->events ()->on_keyboard (kbd_common);

  ui.map->events ()->on_keyboard (ndk::slot (&pimpl::on_kbd_map, this));

  ui.input->on_commit () = ndk::slot (&pimpl::on_command, this);

  stdin_watcher.set (STDIN_FILENO, ev::READ);
  stdin_watcher.set<pimpl, &pimpl::stdin_read_cb> (this);
  stdin_watcher.start ();
}

void
input_handler::pimpl::perform (ndk::panel *sender, action cmd, action arg)
{
  if (state & flags::running)
    {
      send ("command", "run_stop");
      state -= flags::running;
    }
  if (state & flags::firing)
    {
      send ("command", "fire_stop");
      state -= flags::firing;
    }

  switch (cmd)
    {
    case action::none:
      break;

    // direction commands
    case action::north:     send ("command", "north"    ); break;
    case action::northeast: send ("command", "northeast"); break;
    case action::east:      send ("command", "east"     ); break;
    case action::southeast: send ("command", "southeast"); break;
    case action::south:     send ("command", "south"    ); break;
    case action::southwest: send ("command", "southwest"); break;
    case action::west:      send ("command", "west"     ); break;
    case action::northwest: send ("command", "northwest"); break;

    // running
    case action::run:
      send ("command", "run ", int (arg));
      state += flags::running;
      break;

    // firing
    case action::fire:
      send ("command", "fire ", int (arg));
      state += flags::firing;
      break;

    case action::stay_fire:
      send ("command", "stay fire");
      break;

    // control commands
    case action::quit:
      ev_break (EV_DEFAULT_UC_ EVBREAK_ALL);
      break;

    case action::redraw:
      redrawwin (stdscr);
      break;

    case action::resize:
      ui.resize ();
      break;

    case action::maximise:
      if (ui.maximised)
        ui.maximised = nullptr;
      else
        ui.maximised = sender;
      perform (sender, action::resize);
      break;

    case action::input_mode:
      ui.input->visualize ();
      ui.input->focus ();
      break;

    // action commands
    case action::apply:
      send ("command", "apply");
      break;

    case action::take:
      send ("command", "take");
      break;

    case action::repeat:
      if (!last_cmd.empty())
        send ("command", last_cmd);
      break;

    // never happens
    case action::COUNT:
      abort ();
    }
}

ndk::event::result
input_handler::pimpl::on_kbd_common (ndk::keyboard const &ev)
{
  switch (ev.code ())
    {
    case ndk::key::error:
      throw std::runtime_error ("ERR");

    // control actions
    case ndk::key::ctrl_d:
    case ndk::key::q:
      perform (ev.sender (), action::quit);
      return ndk::event::accepted;

    case ndk::key::ctrl_l:
      perform (ev.sender (), action::redraw);
      return ndk::event::accepted;

    case ndk::key::resize:
      perform (ev.sender (), action::resize);
      return ndk::event::accepted;

    case ndk::key::F10:
      perform (ev.sender (), action::maximise);
      return ndk::event::accepted;

    default:
      return ndk::event::ignored;
    }
}

ndk::event::result
input_handler::pimpl::on_kbd_map (ndk::keyboard const &ev)
{
  switch (ev.code ())
    {
    // moving
    case ndk::key::j:
    case ndk::key::k2:
    case ndk::key::down:
      perform (ev.sender (), action::south);
      return ndk::event::accepted;
    case ndk::key::k:
    case ndk::key::k8:
    case ndk::key::up:
      perform (ev.sender (), action::north);
      return ndk::event::accepted;
    case ndk::key::h:
    case ndk::key::k4:
    case ndk::key::left:
      perform (ev.sender (), action::west);
      return ndk::event::accepted;
    case ndk::key::l:
    case ndk::key::k6:
    case ndk::key::right:
      perform (ev.sender (), action::east);
      return ndk::event::accepted;
    case ndk::key::k7:
    case ndk::key::home:
      perform (ev.sender (), action::northwest);
      return ndk::event::accepted;
    case ndk::key::k9:
    case ndk::key::pg_up:
      perform (ev.sender (), action::northeast);
      return ndk::event::accepted;
    case ndk::key::k5:
    case ndk::key::numpad_center:
      perform (ev.sender (), action::stay_fire);
      return ndk::event::accepted;
    case ndk::key::k1:
    case ndk::key::end:
      perform (ev.sender (), action::southwest);
      return ndk::event::accepted;
    case ndk::key::k3:
    case ndk::key::pg_dn:
      perform (ev.sender (), action::southeast);
      return ndk::event::accepted;

    case ndk::key::space:
      perform (ev.sender (), action::apply);
      return ndk::event::accepted;
    case ndk::key::comma:
      perform (ev.sender (), action::take);
      return ndk::event::accepted;
    case ndk::key::dot:
      perform (ev.sender (), action::repeat);
      return ndk::event::accepted;
    case ndk::key::enter:
    case ndk::key::i:
      perform (ev.sender (), action::input_mode);
      return ndk::event::accepted;
    case ndk::key::c:
      ui.input->set_text ("chat ");
      perform (ev.sender (), action::input_mode);
      return ndk::event::accepted;
    case ndk::key::dquote:
      ui.input->set_text ("say ");
      perform (ev.sender (), action::input_mode);
      return ndk::event::accepted;
    case ndk::key::squote:
      ui.input->set_text ("tell ");
      perform (ev.sender (), action::input_mode);
      return ndk::event::accepted;

    // firing
    case ndk::key::J:
    case ndk::key::shift_down:
      perform (ev.sender (), action::fire, action::south);
      return ndk::event::accepted;
    case ndk::key::K:
    case ndk::key::shift_up:
      perform (ev.sender (), action::fire, action::north);
      return ndk::event::accepted;
    case ndk::key::H:
    case ndk::key::shift_left:
      perform (ev.sender (), action::fire, action::west);
      return ndk::event::accepted;
    case ndk::key::L:
    case ndk::key::shift_right:
      perform (ev.sender (), action::fire, action::east);
      return ndk::event::accepted;
    case ndk::key::shift_home:
      perform (ev.sender (), action::fire, action::northwest);
      return ndk::event::accepted;
    case ndk::key::shift_pg_up:
      perform (ev.sender (), action::fire, action::northeast);
      return ndk::event::accepted;
    case ndk::key::shift_end:
      perform (ev.sender (), action::fire, action::southwest);
      return ndk::event::accepted;
    case ndk::key::shift_pg_dn:
      perform (ev.sender (), action::fire, action::southeast);
      return ndk::event::accepted;

    // running
    //case ndk::key::J:
    case ndk::key::ctrl_down:
      perform (ev.sender (), action::run, action::south);
      return ndk::event::accepted;
    //case ndk::key::K:
    case ndk::key::ctrl_up:
      perform (ev.sender (), action::run, action::north);
      return ndk::event::accepted;
    //case ndk::key::H:
    case ndk::key::ctrl_left:
      perform (ev.sender (), action::run, action::west);
      return ndk::event::accepted;
    //case ndk::key::L:
    case ndk::key::ctrl_right:
      perform (ev.sender (), action::run, action::east);
      return ndk::event::accepted;
    case ndk::key::ctrl_home:
      perform (ev.sender (), action::run, action::northwest);
      return ndk::event::accepted;
    case ndk::key::ctrl_pg_up:
      perform (ev.sender (), action::run, action::northeast);
      return ndk::event::accepted;
    case ndk::key::ctrl_end:
      perform (ev.sender (), action::run, action::southwest);
      return ndk::event::accepted;
    case ndk::key::ctrl_pg_dn:
      perform (ev.sender (), action::run, action::southeast);
      return ndk::event::accepted;

    case ndk::key::ctrl_l:
    case ndk::key::F10:
    case ndk::key::resize:
    case ndk::key::tab:
      return ndk::event::ignored;

    case ndk::key::unknown:
      return ndk::event::ignored;

    default:
      if (getenv ("DCLIENT_FATAL"))
        throw ev.code ();
      else
        {
          ui.info->add_text (ndi_color (3, 0), "unknown keycode: 0%03o\n", ev.code ());
          ui.info->visualize ();
          return ndk::event::ignored;
        }
    }
}

void
input_handler::pimpl::on_command (std::string const &cmd)
{
  ui.input->set_text ("");

  if (!cmd.empty ())
    {
      send ("command", cmd);
      last_cmd = cmd;
    }
  else
    {
      ui.map->focus ();
      ui.input->hide ();
      ndk::app->cursor_off (); // FIXME: should be done automatically
    }
}

void
input_handler::pimpl::stdin_read_cb (ev::io &w, int revents)
{
  ndk::app->process (getch ());
}


input_handler::input_handler (communicator &comm, widgets &ui)
  : impl (new pimpl (comm, ui))
{
}

input_handler::~input_handler ()
{
  delete impl;
}
