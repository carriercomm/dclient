#include "dclient/query_handler.h"
#include "dclient/widgets.h"
#include "dclient/widgets/map.h"
#include "dclient/widgets/query.h"

#include "adt/string.h"

#include <ndk/application.hh>

#include <boost/algorithm/string/split.hpp>

query_handler::query_handler ( communicator &comm
                             , widgets &ui
                             , std::string const &username
                             , std::string const &password
                             )
  : sender (comm)
  , ui (ui)
  , username (username)
  , password (password)
{
  ui.query_input->on_commit () = [this] (std::string const &answer) {
    this->send ("reply", answer);
    this->ui.map->focus ();
    this->ui.query_input->set_text ("");
    this->ui.query->hide ();
    ndk::app->cursor_off ();
  };
}

void
query_handler::query (int flags, std::string const &prompt)
{
  if (!username.empty ())
    {
      if (flags == 0 && !strncmp (prompt, "What is your name?"))
        {
          send ("reply", username);
          return;
        }
      else if (flags == 4 && !strncmp (prompt, "What is your password?"))
        {
          send ("reply", password);
          return;
        }
      else if (flags == 4 && !strncmp (prompt, "Please type your password again."))
        {
          send ("reply", password);
          return;
        }
    }
  else
    {
      if (flags == 4 && !strncmp (prompt, "What is your password?"))
        ui.query_input->set_mode (ndk::edit::password);
      else if (flags == 4 && !strncmp (prompt, "Please type your password again."))
        ui.query_input->set_mode (ndk::edit::password);
      else
        ui.query_input->set_mode (ndk::edit::normal);
    }

  std::vector<std::string> lines;
  boost::split (lines, prompt, [](char c) { return c == '\n'; });

  ui.query->update (std::move (lines));
  ui.query->visualize ();
  ui.repack ();

  ui.query_input->focus ();

  ndk::app->update ();
}
