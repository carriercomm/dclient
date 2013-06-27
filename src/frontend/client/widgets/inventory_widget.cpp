#include "dclient/widgets/inventory.h"
#include "dclient/render_glyph.h"

#include "deliantra/data/constants.h"
#include "deliantra/data/glyph.h"
#include "deliantra/data/item.h"
#include "deliantra/data/inventory.h"

#include <ndk/pen.hh>

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/adaptor/sliced.hpp>

#include <algorithm>
#include <cassert>
#include <cstring>

static char const indices[] = " 123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static __attribute__ ((__const__)) int maxidx () { return sizeof indices - 2; }

inventory_widget::inventory_widget (ndk::panel *parent)
  : frame ("inventory", parent)
  , inv (nullptr)
  , at (0)
{
  events ()->on_keyboard (slot (&inventory_widget::at_keyboard, this));
}

int __attribute__ ((__pure__))
inventory_widget::maxinv () const
{
  return std::min (height () - 2, maxidx ());
}

void
inventory_widget::update (data::inventory const &inv, std::string const &title)
{
  this->inv = &inv;
  set_title (title);

  int const inv_size = this->inv->size ();
  if (at > inv_size - 2)
    at = std::max (inv_size - 2, 0);

  visualize ();
}

bool
inventory_widget::draw (ndk::pen &pen, int at, int maxinv, int row, data::item const &item)
{
  return_val_unless (pen.move (1, row), false);

  // previous page
  if (at > 0 && row == 1)
    {
      pen.write ("%c: more…", indices[row]);
      return true;
    }

  // next page
  if (row >= maxinv)
    {
      pen.write ("%c: more…", indices[row]);
      return false;
    }

  pen.write ("%c: [", indices[row]);
  write (pen, data::glyphs[item.face]);
  pen.write ("] ");

  if (item.nrof < 2)
    pen.write (item.singular);
  else
    pen.write ("%d %s", item.nrof, item.plural.c_str ());

  if (item.flags & F_OPEN)    pen.write (" (open)");
  if (item.flags & F_APPLIED) pen.write (" (applied)");
  if (item.flags & F_UNPAID)  pen.write (" (unpaid)");
  if (item.flags & F_MAGIC)   pen.write (" (magic)");
  if (item.flags & F_CURSED)  pen.write (" (cursed)");
  if (item.flags & F_DAMNED)  pen.write (" (damned)");
  if (item.flags & F_LOCKED)  pen.write (" *");

  return true;
}

template<typename ContainerT>
void
inventory_widget::draw (ndk::pen &pen, ContainerT const &items)
{
  int row = 0;
  for (data::item const &item : items)
    if (!draw (pen, at, maxinv (), ++row, item))
      break;
}

void
inventory_widget::draw ()
{
  using boost::adaptors::reversed;
  using boost::adaptors::sliced;

  frame::draw ();

  if (!inv || inv->empty ())
    return;

  ndk::pen pen (*this);

  if (title () == "Floor")
    draw (pen, *inv | sliced (at, inv->size ()) | reversed);
  else
    draw (pen, *inv | sliced (at, inv->size ()));
}

ndk::event::result
inventory_widget::at_keyboard (ndk::keyboard const &ev)
{
  if (!inv)
    return ndk::event::ignored;

  int const inv_size = inv->size ();

  switch (ev.code ())
    {
    case ndk::key::up:
    case ndk::key::k:
      if (at > 0)
        --at;
      else
        return ndk::event::ignored;
      break;

    case ndk::key::down:
    case ndk::key::j:
      if (at < inv_size - 2)
        ++at;
      else
        return ndk::event::ignored;
      break;

    case ndk::key::pg_up:
up:   at -= std::min (maxinv () - 2, int (at));
      break;

    case ndk::key::pg_dn:
      if (inv_size <= 1)
        break;
down: at += std::min (maxinv () - 2, inv_size - at - 2);
      break;

    case ndk::key::home:
      at = 0;
      break;

    case ndk::key::end:
      at = (inv_size > maxinv ())? (inv_size - maxinv () + 1) : 0;
      break;

    default:
      if (char const *const iptr = std::strchr (indices, int (ev.code ())))
        {
          int const index = iptr - indices;
          if (index > maxinv () || at + index > inv_size)
            return ndk::event::ignored;

          if (index == 1)
            goto up;

          if (index == maxinv ())
            goto down;
        }
      return ndk::event::ignored;
    }

  draw ();

  return ndk::event::accepted;
}
