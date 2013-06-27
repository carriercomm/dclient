/* panel.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#include <ndk/pen.hh>
#include <ndk/palette.hh>

#include <ndk/panel.hh>
#include <ndk/application.hh>

#include <algorithm>

ndk::panel::panel (panel *parent)
  : parent_ (parent)
  , events_ (parent ? parent->events () : 0, this)
{
  /* create panel object */
  panel_ = new_panel (win ());
  if (!panel_)
    throw std::runtime_error ("new_panel failed");
  if (parent_)
    {
      move_panel (panel_, parent_->y (), parent_->x ());        /* move to default position relative to father */
      parent_->add_child (this);          /* append itself to the father */
    }
  hide ();      /* hide widget by default */

  events ()->on_system (ndk::slot (&panel::at_system, this));
}

ndk::panel::~panel ()
{
  if (parent_)
    parent_->del_child (this);                 /* remove child from the parent's list */
  del_panel (panel_);
}

void
ndk::panel::add_child (panel *item)
{
  childs_.push_back (item);      /* append a new child to the list */
}

void
ndk::panel::del_child (panel *item)
{
  childs_.erase (remove (childs_.begin (),
                         childs_.end (),
                         item), childs_.end ());
}

void
ndk::panel::top ()
{
  if (hidden ())
    return;                   /* we should skip this function if panel hidden */
  return_unless (::top_panel (panel_) == OK);
  for (panel *p : childs_)
    p->top ();
}

void
ndk::panel::bottom ()
{
  if (hidden ())
    return;                   /* we should skip this function if panel hidden */
  return_unless (::bottom_panel (panel_) == OK);
  for (panel *p : childs_)
    p->bottom ();
}

void
ndk::panel::show ()
{
  /* we should skip this function if panel already visible */
  if (hidden ())
    return_unless (::show_panel (panel_) == OK);
  for (panel *p : childs_)
    p->show ();
}

void
ndk::panel::hide ()
{
  /* we should skip this function if panel already hidden */
  if (!hidden ())
    return_unless (::hide_panel (panel_) == OK);
  for (panel *p : childs_)
    p->hide ();
}

bool
ndk::panel::hidden () const
{
  return ::panel_hidden (panel_) == TRUE;
}

bool
ndk::panel::higher (panel const &other) const
{
  PANEL *below = panel_;

  while ((below = panel_below (below)) != 0)
    if (below == other.panel_)
      return true;
  return false;
}

void
ndk::panel::prepare (bool recursive)
{
  draw ();
  if (recursive)
    for (panel *child : childs_)
      child->prepare ();
}

void
ndk::panel::visualize (bool recursive)
{
  prepare (recursive);
  show ();
}

void
ndk::panel::move (int new_x, int new_y, coords_type type)
{
  /* save difference */
  int const diffx = new_x - x ();
  int const diffy = new_y - y ();
  bool const abs_type = type == absolute || type == rel_parent;

  if (type == rel_parent && parent ())
    {
      new_x += parent ()->x ();
      new_y += parent ()->y ();
    }

  move_panel ( panel_
             , abs_type ? new_y : y () + new_y
             , abs_type ? new_x : x () + new_x
             );

  /* childs_ always moving in relative coordinates */
  for (panel *p : childs_)
    p->move (abs_type ? diffx : new_x, abs_type ? diffy : new_y);
}

/**
 * @todo add support for palette
 */
void
ndk::panel::draw ()
{
  pen (*this).fill (palette::instance ().find (colors::panel)->second);
}

int
ndk::panel::padding () const
{
  return 0;
}

void
ndk::panel::emit (int type)
{
  events_.emit (type);
}

ndk::engine *
ndk::panel::events ()
{
  return &events_;
}

void
ndk::panel::in_focus (panel &other)
{
  events_.in_focus (other.events ());
}

void
ndk::panel::focus ()
{
  if (parent ())
    {
      dynamic_cast<panel &> (*parent ()).focus ();
      dynamic_cast<panel &> (*parent ()).in_focus (*this);
    }
  else
    ndk::app->in_focus (*this);
}

ndk::event::result
ndk::panel::at_system (event const &ev)
{
  if (!ev.sender_ && engine::enter_focus == ev.type_)
    {
      top ();
      return event::accepted;
    }
  return event::ignored;
}
