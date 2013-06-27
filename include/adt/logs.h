#pragma once

#include <fstream>

struct logstream
{
  logstream (char const *file, int line, char const *func);
  ~logstream ();

  logstream const &operator () (char const *channel = "dclient") const;

  static std::ofstream fh;

  char const *file;
  long line;
  char const *func;
};

template<typename T>
static inline logstream const &
operator << (logstream const &log, T const &v)
{
  log.fh << v;
  return log;
}

#define LOG logstream (__FILE__, __LINE__, __func__)
