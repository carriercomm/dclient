/* align.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#include <stdexcept>

#include <ndk/align.hh>

int
ndk::align::pos (int internal_size, int external_size, align::type align)
{
  switch (align)
    {
    case left:
      return 0;
    case right:
      return external_size - internal_size;
    case center:
      return (external_size - internal_size) / 2;
    default:
      throw std::runtime_error ("unknown align type");
    }
}
