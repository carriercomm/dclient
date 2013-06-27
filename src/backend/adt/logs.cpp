#include "adt/logs.h"

#include <boost/algorithm/string/case_conv.hpp>

#include <iomanip>
#include <cstring>

#include <sys/time.h>

std::ofstream logstream::fh ("client.log");

static timeval
now ()
{
  timeval now;
  gettimeofday (&now, NULL);
  return now;
}

static timeval const boot = now ();

logstream::logstream (char const *file, int line, char const *func)
  : file (file)
  , line (line)
  , func (func)
{
  timeval const now = ::now ();
  timeval sub;
  timersub (&now, &boot, &sub);

  fh << '[' << std::setfill (' ') << std::setw (3) << sub.tv_sec << '.' << std::setfill ('0') << std::setw (6) << sub.tv_usec << "] ";
}

logstream const &
logstream::operator () (char const *channel) const
{
  fh << '<' << boost::to_upper_copy (std::string (channel)) << "> ";
  fh << strrchr (file, '/') + 1 << ':' << line << '(' << func << "): ";
  return *this;
}

logstream::~logstream ()
{
  fh.flush ();
}


void
log_failure (char const *file, int line, char const *func, char const *cond)
{
  logstream (file, line, func) ("error") << cond << '\n';
}
