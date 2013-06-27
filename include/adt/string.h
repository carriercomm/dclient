#pragma once

#include <cstring>
#include <string>

template<size_t N>
static int
strncmp (std::string const &s1, char const (&s2)[N])
{
  return s1.compare (0, N - 1, s2);
}

template<size_t N>
static int
strncmp (char const *s1, char const (&s2)[N])
{
  return strncmp (s1, s2, N - 1);
}
