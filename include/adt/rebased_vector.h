#pragma once

#include <cstddef>
#include <deque>

template<typename T>
struct rebased_vector
{
  typedef std::deque<T> container_type;

  typedef typename container_type::      iterator       iterator;
  typedef typename container_type::const_iterator const_iterator;

  T       &front ()       { return data.front (); }
  T const &front () const { return data.front (); }

  T       &at (int n)       { return data.at (offset + n); }
  T const &at (int n) const { return data.at (offset + n); }

  T       &operator[] (int n)       { return data[offset + n]; }
  T const &operator[] (int n) const { return data[offset + n]; }

  bool has (int n) const { return offset + n >= 0 && offset + n < size (); }

  void clear () { return data.clear (); }

  int size () const
  {
    return data.size ();
  }

  void ensure (int x)
  {
    while (offset + x < 0)
      {
        data.emplace_front ();
        ++offset;
      }
    while (offset + x >= size ())
      {
        data.emplace_back ();
      }
  }

  int low  () const { return -offset; }
  int high () const { return data.size () + low (); }

        iterator begin ()       { return data.begin (); }
        iterator end   ()       { return data.end   (); }
  const_iterator begin () const { return data.begin (); }
  const_iterator end   () const { return data.end   (); }

  rebased_vector ()
    : offset (0)
  {
  }

private:
  long offset;
  container_type data;
};
