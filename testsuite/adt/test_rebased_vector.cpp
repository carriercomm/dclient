#include "adt/rebased_vector.h"

#include <iostream>

int
main ()
{
  rebased_vector<int> v;

  v.ensure (-10);
  v.ensure (10);
  v.at (-10) = 2;
  v.at (10) = 2;

  for (int i = v.low (); i < v.high (); i++)
    std::cout << i << ": " << v.at (i) << "\n";
}
