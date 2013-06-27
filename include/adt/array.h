#pragma once

#include <array>

template<typename T, typename... Args>
std::array<T, sizeof... (Args) + 1>
array (T const &first, Args const &...rest)
{
  return std::array<T, sizeof... (Args) + 1> { { first, rest... } };
}
