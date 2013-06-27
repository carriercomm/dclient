#pragma once

#include <initializer_list>
#include <utility>

template<typename T>
struct index_map
{
  typedef unsigned char				key_type;
  typedef T					mapped_type;
  typedef std::pair<key_type, mapped_type>	value_type;
  typedef mapped_type				array_type[UCHAR_MAX];

  index_map (std::initializer_list<value_type> const &init)
  {
    for (value_type const &pair : init)
      data[pair.first] = pair.second;
  }

  mapped_type const &operator [] (key_type n) const { return data[n]; }

private:
  array_type data;
};
