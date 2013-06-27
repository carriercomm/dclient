struct widgets;
struct communicator;

struct input_handler
{
  input_handler (communicator &comm, widgets &ui);
  ~input_handler ();

private:
  struct pimpl;
  pimpl *impl;
};
