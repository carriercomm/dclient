/* events.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#pragma once

#include "key.hh"

namespace ndk
{
  struct panel;
}

namespace ndk
{
  struct mouse
  {
    MEVENT mev;

    int x () const { return mev.x; }
    int y () const { return mev.y; }
    bool button1 () const { return (mev.bstate & BUTTON1_CLICKED) == BUTTON1_CLICKED; }
    bool button2 () const { return (mev.bstate & BUTTON2_CLICKED) == BUTTON2_CLICKED; }
    bool button3 () const { return (mev.bstate & BUTTON3_CLICKED) == BUTTON3_CLICKED; }

    // check: was the mouse click over a given panel?
    bool over (panel const &p) const;
  };

  struct keyboard       /**< event from the keyboard */
  {
    explicit keyboard (key code, panel *sender = 0)
      : sender_ (sender)
      , code_ (code)
    {
    }

    panel *sender () const { return sender_; }
    key code () const { return code_; }

  private:
    panel *sender_;
    key const code_;             /**< code of the button */
  };

  struct engine;

  struct event       /**< basic NDK internal event  */
  {
    enum result
    {
      ignored = false,                                   /**< event ignored */
      accepted = true                   /**< event successfully proceed */
    };

    event (int t, engine *sender = 0)
      : sender_ (sender)
      , type_ (t)
    {
    }

    engine *sender_;             /**< sender of the envent */
    int type_;             /**< one of the possible types of the event */
  };


  template<typename Ret, typename Tp, typename ArgT>
  struct slot_fn
  {
    slot_fn (Ret (Tp::*f)(ArgT), Tp *obj)
      : f_ (f)
      , obj_ (obj)
    {
    }

    Ret operator () (ArgT ev) { return (obj_->*f_)(ev); }

    Ret (Tp::*f_)(ArgT);
    Tp *obj_;
  };

  template<typename Tp1, typename Tp2>
  struct slot_fnb
  {
    slot_fnb (void (Tp1::*f)(), Tp2 *obj)
      : f_ (f)
      , obj_ (obj)
    {
    }

    void operator () () { (obj_->*f_)(); }

    void (Tp1::*f_)();
    Tp2 *obj_;
  };

  template<typename Tp1, typename Tp2, typename ArgT>
  struct slot_fnb_1
  {
    slot_fnb_1 (void (Tp1::*f)(ArgT), Tp2 *obj, ArgT arg)
      : f_ (f)
      , obj_ (obj)
      , arg_ (arg)
    {
    }

    void operator () () { (obj_->*f_)(arg_); }

    void (Tp1::*f_)(ArgT);
    Tp2 *obj_;
    ArgT arg_;
  };

  template<typename Ret, typename Tp, typename ArgT>
  slot_fn<Ret, Tp, ArgT>
  slot (Ret (Tp::*f)(ArgT), Tp *obj)
  {
    return slot_fn<Ret, Tp, ArgT> (f, obj);
  }

  template<typename Tp1, typename Tp2>
  slot_fnb<Tp1, Tp2>
  slot (void (Tp1::*f)(), Tp2 *obj)
  {
    return slot_fnb<Tp1, Tp2> (f, obj);
  }

  template<typename Tp1, typename Tp2, typename ArgT>
  slot_fnb_1<Tp1, Tp2, ArgT>
  slot (void (Tp1::*f)(ArgT), Tp2 *obj, ArgT arg)
  {
    return slot_fnb_1<Tp1, Tp2, ArgT> (f, obj, arg);
  }
}
