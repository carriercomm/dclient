#include "config.h"

#include "adt/logs.h"

void log_failure (char const *file, int line, char const *func, char const *cond);

#define LOC __FILE__, __LINE__, __func__

#define return_unless(cond)             \
  do {                                  \
    if (!(cond))                        \
      { log_failure (LOC, #cond);       \
        return; }                       \
  } while (0)

#define return_val_unless(cond, val)    \
  do {                                  \
    if (!(cond))                        \
      { log_failure (LOC, #cond);       \
        return val; }                   \
  } while (0)
