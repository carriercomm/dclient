#include "deliantra/exception.h"

int
main (int argc, char *argv[])
try
{
  setlocale (LC_ALL, "");

  if (argc == 2 && !strcmp (argv[1], "--version"))
    {
      printf ("%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
      return EXIT_SUCCESS;
    }

  if (argc < 3 || !strcmp (argv[1], "--help"))
    {
      printf ("Usage: %s <host> <port> [user] [password]\n", PACKAGE);
      return EXIT_SUCCESS;
    }

  std::string server, port, user, pass;

  server = argv[1];
  port   = argv[2];
  if (argc == 5)
    {
      user = argv[3];
      pass = argv[4];
    }

  start_game (server, port, user, pass);

  return EXIT_SUCCESS;
}
catch (exit_success const &e)
{
  LOG ("exception") << e.what () << "\n";
  printf ("exiting: %s\n", e.what ());
  return EXIT_SUCCESS;
}
catch (std::exception const &e)
{
  LOG ("exception") << e.what () << "\n";
  printf ("exception caught: %s\n", e.what ());
  return EXIT_FAILURE;
}
catch (int i)
{
  LOG ("exception") << '0' << std::oct << std::setfill ('0') << std::setw (3) << i << "\n";
  printf ("exception caught: 0%03o\n", i);
  return EXIT_FAILURE;
}
catch (std::string const &s)
{
  LOG ("exception") << s << "\n";
  printf ("exception caught: %s\n", s.c_str ());
  return EXIT_FAILURE;
}
catch (char const *s)
{
  LOG ("exception") << s << "\n";
  printf ("exception caught: %s\n", s);
  return EXIT_FAILURE;
}
catch (...)
{
  LOG ("exception");
  puts ("exception caught");
  return EXIT_FAILURE;
}
