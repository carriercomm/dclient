#include "tinput_handler.h"

#include "trenderer.h"

#include "deliantra/exception.h"

#include "adt/string.h"

input_handler::input_handler (communicator &comm)
  : sender (comm)
{
  stdin_watcher.set (STDIN_FILENO, ev::READ);
  stdin_watcher.set<input_handler, &input_handler::stdin_read_cb> (this);
  stdin_watcher.start ();
}

input_handler::~input_handler ()
{
}

void
input_handler::stdin_read_cb (ev::io &w, int revents)
{
  char buf[BUFSIZ];
  ssize_t len = read (w.fd, buf, sizeof buf);

  if (len < 0)
    throw "read error (stdin): " + std::string (strerror (errno));
  if (len == 0)
    throw exit_success ("EOF from stdin");

  return_unless (buf[len - 1] == '\n');
  buf[len - 1] = '\0';

  if (buf[0] == '/')
    {
      if (!strncmp (buf, "/dump-map"))
        rend->draw_map ();
      else
        send (buf + 1, size_t (len) - 2);
    }
  else
    send ("command", std::string (buf, len - 1));
}
