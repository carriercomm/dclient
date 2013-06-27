#pragma once

#include <stdexcept>

struct exit_success
  : std::runtime_error
{
  template<typename ...Args>
  exit_success (Args &&...args)
    : std::runtime_error (args...)
  {
  }
};
