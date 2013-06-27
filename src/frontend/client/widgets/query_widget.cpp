#include "dclient/widgets/query.h"

#include "deliantra/data/counters.h"
#include "deliantra/data/map.h"
#include "deliantra/data/statistics.h"

#include <ndk/pen.hh>

query_widget::query_widget (ndk::panel *parent)
  : frame ("", parent)
{
}

void
query_widget::update (string_vector &&query)
{
  this->query = query;

  std::string::size_type maxlen = 0;
  for (std::string const &line : query)
    maxlen = std::max (maxlen, line.size ());

  resize (maxlen + 2, query.size () + 4);
}

void
query_widget::draw ()
{
  frame::draw ();

  ndk::pen pen (*this);
  int row = 0;
  for (std::string const &line : query)
    {
      return_unless (pen.move (1, ++row));
      pen.write (line);
    }
}
