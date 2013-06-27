/* transport.cc
 * This file is part of NDK++ library
 * Copyright (c) 2003 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */

#include "live.h"

#include <ndk/transport.hh>

#include <ndk/application.hh>
#include <ndk/events.hh>
#include <ndk/keymap.hh>

#include <ndk/panel.hh>

#include <adt/string.h>

#include <curses.h>

#include <algorithm>

using namespace ndk;

namespace
{
  template<typename EventT>
  struct run_impl
  {
    /**< functor to run callback functions */
    run_impl (EventT const &e)
      : e_ (e)
    {
    }

    template<typename CallbackT>
    event::result operator () (CallbackT &callback)
    {
      return callback (e_);
    }

    EventT const &e_;
  };

  template<typename EventT>
  run_impl<EventT>
  run_callback (EventT const &e)
  {
    return run_impl<EventT> (e);
  }

  template<typename Ret, typename Tp, typename Arg1T>
  struct mem_fun1_impl
  {
    /**< simple helper binder of member function */
    mem_fun1_impl (Ret (Tp::*f)(Arg1T), Arg1T arg)
      : f_ (f)
      , arg_ (arg)
    {
    }

    Ret operator () (Tp *obj)
    {
      return (obj->*f_)(arg_);
    }

    Ret (Tp::*f_)(Arg1T);
    Arg1T arg_;
  };

  template<typename Ret, typename Tp, typename Arg1T, typename Arg2T>
  struct mem_fun2_impl
  {
    /**< simple helper binder of member function */
    mem_fun2_impl (Ret (Tp::*f)(Arg1T, Arg2T), Arg1T arg1, Arg2T arg2)
      : f_ (f)
      , arg1_ (arg1)
      , arg2_ (arg2)
    {
    }

    Ret operator () (Tp *obj)
    {
      return (obj->*f_)(arg1_, arg2_);
    }

    Ret (Tp::*f_)(Arg1T, Arg2T);
    Arg1T arg1_;
    Arg2T arg2_;
  };
}


bool
mouse::over (panel const &p) const
{
  if (p.hidden ())
    return false;
  else
    return p.x () <= x () && x () <= p.right_x  () - 1 &&
           p.y () <= y () && y () <= p.bottom_y () - 1;

}

engine::engine (engine *parent, panel *geometry)
  : geometry_ (geometry)
  , in_focus_ (this)
  , accept_ (true)
{
  parent_ = parent ? parent : app->events ();     /* by default head is application */
  parent_->add_child (this);
}

engine::engine ()
  : parent_ (0)
  , geometry_ (0)
  , in_focus_ (this)
  , accept_ (true)
{
}

engine::~engine ()
{
  if (parent_)
    parent_->del_child (this);
}

void
engine::in_focus (engine *item)
{
  if (find (childs_.begin (), childs_.end (), item) != childs_.end ())
    {
      in_focus_ = item;
      in_focus_->system (event (enter_focus));         /* notify item */
    }
  else
    throw std::runtime_error ("unable to set focus to unknown widget");
}

engine *
engine::head ()        /**< @return main head of the transport tree */
{
  engine *head = this;       /* find local event loop head */

  while (head->parent_)
    head = head->parent_;
  return head;
}

void
engine::update ()
{
#if LIVE
  update_panels ();      /* update current status */
  return_unless (doupdate () == OK);
#endif
}

void
engine::process (int const ch)
{
  key code = parsekey (esc_, ch);
  if (code == key::mouse)
    {
      mouse ev = getmouse (esc_);
      process (ev);
    }
  else if (code != key::intermediate)
    {
      keyboard ev (code);
      process (ev);
    }

#if LIVE
  return_unless (doupdate () == OK);      /* commit all changes to the screen */
#endif
}

void
engine::on_keyboard (keyboard_event const &callback, bool front)
{
  if (front)
    keyboard_.push_front (callback);
  else
    keyboard_.push_back (callback);
}

void
engine::on_mouse (mouse_event const &callback, bool front)
{
  if (front)
    mouse_.push_front (callback);
  else
    mouse_.push_back (callback);
}

void
engine::on_system (system_event const &callback, bool front)
{
  if (front)
    system_.push_front (callback);
  else
    system_.push_back (callback);
}

void
engine::rat (mouse const &ev)
{
  for (mouse_event &m : mouse_)
    m (ev);
}

event::result
engine::kbd (keyboard const &kev)
{
  event::result res = event::ignored;
  keyboard const ev (kev.code (), geometry_);
  kbd_events::iterator it = find_if (keyboard_.begin (),
                                     keyboard_.end (),
                                     run_callback (ev));

  if (it != keyboard_.end ())
    {
      res = event::accepted;
      for_each (++it,
                keyboard_.end (),
                run_callback (ev));
    }

  return res;
}

event::result
engine::system (event const &ev)
{
  event::result res = event::ignored;
  sys_events::iterator it = find_if (system_.begin (),
                                     system_.end (),
                                     run_callback (ev));

  if (it != system_.end ())
    {
      res = event::accepted;
      for_each (++it,
                system_.end (),
                run_callback (ev));
    }

  return res;
}

void
engine::add_child (engine *p)
{
  childs_.push_back (p);
}

void
engine::del_child (engine *p)
{
  childs_.erase (remove (childs_.begin (),
                         childs_.end (),
                         p), childs_.end ());
}

void
engine::little_switch (engine *item)
{
  if (item == in_focus_ || !item->accept ())
    return;                                                /* nothing to switch */

  /* all permissions ok - let's switch focus now */
  if (in_focus_ != this)
    in_focus_->leave_all ();
  in_focus_ = item;
  if (in_focus_ != this)
    in_focus_->enter_all ();

#if LIVE
  update_panels ();      /* update panel's status */
#endif
}

void
engine::big_switch (engine *item)
{
  child_items list;           /* hierarchy we need to walk to produce a big switch */

  /* first cycle determine widgets we need to switch */
  for (; item != this; item = item->parent_)
    list.push_back (item);

  for (child_items::reverse_iterator it = list.rbegin (); it != list.rend (); it++)
    (*it)->parent_->little_switch (*it);
}

void
engine::switch_to (engine *item)
{
  head ()->big_switch (item);
}

void
engine::find_over (mouse const &ev, child_items *list)
{
  if (geometry_ && ev.over (*geometry_))
    list->push_back (this);

  for (engine *child : childs_)
    child->find_over (ev, list);
}

void
engine::process (mouse const &ev)
{
  /* walk throw the all objects and find all objects under mouse pointer */
  child_items list;

  find_over (ev, &list);

  if (list.empty ())
    {
      flash ();
      return;
    }

  /* then find widget over all visible widgets */
  child_items::iterator top = max_element (list.begin (),
                                           list.end (),
                                           higher);

  big_switch (*top);      /* then made a big switch to top widget */

  /*
   * let's widget process it's event
   * find latest widget who can process mouse event
   */
  engine *current = *top;
  while (!current->accept () && current->parent_)
    current = current->parent_;
  if (current->parent_)
    current->rat (ev);
}

void
engine::find_leafs (child_items *list)
{
  child_items active_childs;

  for (engine *child : childs_)
    child->find_leafs (&active_childs);

  /*
   * if we cannot find active childs, but have active father
   * then add father if it's not hidden now
   */
  if (active_childs.empty () && accept_ && geometry_ && !geometry_->hidden ())
    list->push_back (this);
  else
    /* append active childs to the big list */
    list->insert (list->end (), active_childs.begin (), active_childs.end ());
}

void
engine::process (keyboard const &ev)
{
  /*
   * call at_event only for itself else call process event to pass messages
   * to subwidgets, last widget always have in_focus_ equals "this"
   */
  engine *current = this;

  if (ev.code () == key::error)
    throw std::runtime_error ("ERR");

  while (current->in_focus_ != current)
    current = current->in_focus_;
  if (current->kbd (ev) != event::accepted) /* if widget ignore processing then we can process it itself */
    if (ev.code () == key::tab)
      {
        child_items list;
        find_leafs (&list);                /* collect all leaves of the transport subtree //FIXME: start search not from head, but from current in_focus */

        if (list.empty ())
          return;                                  /* nothing to switch to */

        engine *next;                 /* calculate next candidate to switch */
        child_items::iterator it = find (list.begin (), list.end (), current);
        if (it == list.end ())                /* first item by default */
          next = list.at (0);
        else
          next = (++it == list.end ()) ? *list.begin () : *it;

        big_switch (next);
      }
}

void
engine::emit (int type)
{
  head ()->process_system (event (type, this));
}

event::result
engine::process_system (event const &ev)
{
  /* find are some child accept this event */
  child_items::const_iterator item =
    find_if (childs_.begin (),
             childs_.end (),
             mem_fun1_impl<event::result, engine, event const &> (&engine::process_system, ev));

  return (item != childs_.end ()) ? event::accepted : system (ev);
}

/* send event to object and all it's childs */
void
engine::leave_all ()
{
  system (event (leave_focus));
  for (engine *child : childs_)
    child->leave_all ();
}

void
engine::enter_all ()
{
  engine *current = this;

  while (current->in_focus_ != current)
    {
      current->system (event (enter_focus));
      current = current->in_focus_;
    }
  current->system (event (enter_focus));
}

void
engine::set_accept (bool val)
{
  accept_ = val;
}

bool
engine::accept () const
{
  return accept_;
}

bool
engine::higher (engine *lhs, engine *rhs)
{
  return rhs->geometry_->higher (*lhs->geometry_);
}
