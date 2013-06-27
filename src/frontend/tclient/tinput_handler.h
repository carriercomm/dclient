#include "deliantra/sender.h"

#include <ev++.h>

struct renderer;

struct input_handler
  : sender
{
  input_handler (communicator &comm);
  ~input_handler ();

  renderer *rend;

private:
  void stdin_read_cb (ev::io &w, int revents);

  ev::io stdin_watcher;
};
