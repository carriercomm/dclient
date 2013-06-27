/* support.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#pragma once

#include <ndk/support.hh>

#include <memory>
#include <stdexcept>
#include <string>

namespace ndk
{
  template<class T>
  class singleton_holder
  {
    /**< simple Meyers' style singleton */
  public:
    typedef T obj_type;
    static T &
    instance ()                      /**< @return instance of the object */
    {
      static T obj;

      return obj;
    }

  private:
    /* all this operations are unavailable to ordinal users */
    singleton_holder ();
    singleton_holder (const singleton_holder &s);
    singleton_holder &operator = (const singleton_holder &s);
    ~singleton_holder ();
  };


  struct NoArg { };

  template<typename RetT, typename ArgT>
  struct functor_impl
  {
    // general polymorphic functor interface
    virtual ~functor_impl ()
    {
    }

    virtual RetT operator () (ArgT) = 0;
  };

  template<typename RetT>
  struct functor_impl<RetT, NoArg>
  {
    // general polymorphic functor interface
    virtual ~functor_impl ()
    {
    }

    virtual RetT operator () () = 0;
  };

  template<typename FunctorT, typename RetT, typename ArgT>
  struct functor_impl_arb
    : public functor_impl<RetT, ArgT>
  {
    // special interface version for arbitrary functor
    functor_impl_arb (const FunctorT &fun)
      : fun_ (fun)
    {
    }

    RetT operator () (ArgT arg)
    {
      return fun_ (arg);
    }

    FunctorT fun_;             // internal copy of the functor
  };

  template<typename FunctorT, typename RetT>
  struct functor_impl_arb<FunctorT, RetT, NoArg>
    : public functor_impl<RetT, NoArg>
  {
    // special interface version for arbitrary functor
    functor_impl_arb (const FunctorT &fun)
      : fun_ (fun)
    {
    }

    RetT operator () ()
    {
      return fun_ ();
    }

    FunctorT fun_;             // internal copy of the functor
  };

  template<typename RetT, typename ArgT = NoArg>
  struct functor
  {
    // generalized functor for ndk callbacks
    typedef functor_impl<RetT, ArgT> impl_type;
    typedef std::shared_ptr<impl_type> invoker_type;

    template<typename FunctorT>
    functor (const FunctorT &fun)
      // initialize with functor type
      : invoker_ (new functor_impl_arb<FunctorT, RetT, ArgT> (fun))
    {
    }

    RetT operator () (ArgT arg)
    {
      // run action
      if (invoker_.get ())
        return (*invoker_.get ()) (arg);
      throw std::runtime_error ("calling empty functor");
    }

    template<typename FunctorT>
    functor<RetT, ArgT> &
    operator = (const FunctorT fun)
    {
      invoker_ = new functor_impl_arb<FunctorT, RetT, ArgT> (fun);
      return *this;
    }

    invoker_type invoker_;
  };

  template<typename RetT>
  struct functor<RetT, NoArg>
  {
    // generalized functor for ndk callbacks
    typedef functor_impl<RetT, NoArg> impl_type;
    typedef std::shared_ptr<impl_type> invoker_type;

    template<typename FunctorT>
    functor (const FunctorT &fun)
      //  initialize with functor type
      : invoker_ (new functor_impl_arb<FunctorT, RetT, NoArg> (fun))
    {
    }

    RetT operator () ()
    {
      // run action
      if (invoker_.get ())
        return (*invoker_.get ())(); throw std::runtime_error ("calling empty functor");
    }

    template<typename FunctorT>
    functor<RetT, NoArg> &
    operator = (const FunctorT fun)
    {
      invoker_ = new functor_impl_arb<FunctorT, RetT, NoArg> (fun);
      return *this;
    }

    invoker_type invoker_;
  };
}
