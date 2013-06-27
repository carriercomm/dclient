#pragma once

#include "key.hh"

#include <string>

namespace ndk
{
  struct mouse;

  bool has_keymap ();
  key parsekey (std::string &esc, int ch);
  mouse getmouse (std::string &esc);
}
