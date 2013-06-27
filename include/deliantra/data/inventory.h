#pragma once

#include "types.h"

#include <vector>

namespace data
{
  struct item;

  struct inventory
    : private std::vector<item>
  {
    typedef std::vector<item> vector_type;

    using vector_type::reverse_iterator;
    using vector_type::const_reverse_iterator;

    using vector_type::iterator;
    using vector_type::const_iterator;

    using vector_type::begin;
    using vector_type::end;

    using vector_type::rbegin;
    using vector_type::rend;

    using vector_type::empty;
    using vector_type::size;

    using vector_type::at;
    using vector_type::back;
    using vector_type::front;

    using vector_type::push_back;

    using vector_type::erase;
  };
}
