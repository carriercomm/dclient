#include "tquery_handler.h"

#include "adt/string.h"

#include <boost/algorithm/string/split.hpp>

#include <iostream>

query_handler::query_handler ( communicator &comm
                             , std::string const &username
                             , std::string const &password
                             )
  : sender (comm)
  , username (username)
  , password (password)
{
}

void
query_handler::query (int flags, std::string const &prompt)
{
  if (!username.empty ())
    {
      if (flags == 0 && !strncmp (prompt, "What is your name?"))
        {
          send ("reply", username);
          return;
        }
      else if (flags == 4 && !strncmp (prompt, "What is your password?"))
        {
          send ("reply", password);
          return;
        }
      else if (flags == 4 && !strncmp (prompt, "Please type your password again."))
        {
          send ("reply", password);
          return;
        }
    }

  std::cout << "query: " << prompt << '\n';
}
