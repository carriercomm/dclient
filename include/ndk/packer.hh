/* packer.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#ifndef NDK_PACKER_HH__
#define NDK_PACKER_HH__

#include <algorithm>
#include <functional>
#include <vector>

#include <ndk/align.hh>
#include <ndk/panel.hh>
#include <ndk/support.hh>

namespace ndk
{
  namespace pack       /**< packer strategies */
  {
    struct vertical             /**<  vertical packing strategy */
    {
      static int align_coord (panel *p)
      {
        return p->x ();
      }

      static int align_size (panel *p)
      {
        return p->width ();
      }

      static void align_move (panel *p, int pos)
      {
        p->move (pos, p->y (), absolute);
      }

      static void align_resize (panel *p, int size)
      {
        const bool need_update = (p->width () != size);

        p->resize (size, p->height ());
        if (need_update)
          p->draw ();
      }

      static int line_coord (panel *p)
      {
        return p->y ();
      }

      static int line_size (panel *p)
      {
        return p->height ();
      }

      static void line_move (panel *p, int pos)
      {
        p->move (p->x (), pos, absolute);
      }

      static void resize (panel *p, int align_size, int line_size)
      {
        p->resize (align_size, line_size);
      }
    };

    struct horizontal             /**<  horizontal packing strategy */
    {
      static int align_coord (panel *p)
      {
        return p->y ();
      }

      static int align_size (panel *p)
      {
        return p->height ();
      }

      static void align_move (panel *p, int pos)
      {
        p->move (p->x (), pos, absolute);
      }

      static void align_resize (panel *p, int size)
      {
        const bool need_update = (p->height () != size);

        p->resize (p->width (), size);
        if (need_update)
          p->draw ();
      }

      static int line_coord (panel *p)
      {
        return p->x ();
      }

      static int line_size (panel *p)
      {
        return p->width ();
      }

      static void line_move (panel *p, int pos)
      {
        p->move (pos, p->y (), absolute);
      }

      static void resize (panel *p, int align_size, int line_size)
      {
        p->resize (line_size, align_size);
      }
    };

    template<class T>
    struct def_resizer             /**< default resize policy for the packer object */
    {
      static void fix_item (panel *p, int, int pos)
      {
        T::align_move (p, pos);
      }

      static void fix_panel (panel *p, int align_size, int line_size)
      {
        T::resize (p, align_size, line_size);
      }

      static void draw (panel *p)
      {
        p->draw ();
      }
    };

    template<class T>
    struct no_resizer             /**<  * this policy don't made resize of the object */
    {
      static void fix_item (panel *p, int size, int pos)
      {
        T::align_move (p, pos);
        T::align_resize (p, size);
      }

      static void fix_panel (panel *, int, int)
      {
      }

      static void draw (panel *)
      {
      }
    };
  }

  template<class T>
  bool
  wider_panel (panel *p1, panel *p2)
  {
    /**< compare panels, who is wider */
    return T::align_size (p1) < T::align_size (p2);
  }

  /**
   * general template to the auto packer/aligner of widgets
   * packer algorithm:
   * with resizer:
   * 1. find widget with maximum size (max)
   * 2. main widget size is max + 2*padding
   * 3. align all widgets and place it with begining from padding
   * 4. resize main to size max + 2*padding, and total widgets length+2*padding
   * without resizer:
   * 1. find widget with maximum size
   * 2. align all widgets, according max widget size and with begining from padding
   * 3. resize all widgets to the "main - 2*padding" if we need it
   */
  template<class T, template<class> class resizer = pack::def_resizer>
  class packer
  {
    typedef std::vector<panel *>        widget_vector;
    typedef resizer<T>                  resizer_type;

  public:
    packer (panel *main, align::type alignment, int padding = 0)
      : main_ (main)
      , padding_ (padding)
      , align_ (alignment)
    {
    }

    void add (panel *item)
    {
      widgets_.push_back (item);
      resizer_type::draw (item);                  /* in auto resize method we need to draw widgets to know it's real sizes */
    }

    void pack ()                 /**< move and align widgets */
    {
      if (widgets_.empty ())
        return;

      /* first made alignment */
      widget_vector::const_iterator max_item =
        max_element (widgets_.begin (), widgets_.end (), wider_panel<T> );
      const int align_size = T::align_size (*max_item) + main_->padding () * 2;
      for (panel *widget : widgets_)
        do_align (widget, T::align_size (*max_item));

      int pos = main_->padding ();
      for (panel *widget : widgets_)
        do_move (widget, pos);
      const int line_size = pos - padding_ + main_->padding ();

      resizer_type::fix_panel (main_, align_size, line_size);                  /* try to fix size of the panel */
    }

  private:
    void do_align (panel *p, int max)                 /**< align panel's possition according current aligment */
    {
      resizer_type::fix_item (p, T::align_size (main_) - 2 * main_->padding (),
                              T::align_coord (main_) +
                              main_->padding () +
                              align::pos (T::align_size (p), max, align_));
    }

    void do_move (panel *p, int &pos)                 /**< move next item according current packing */
    {
      T::line_move (p, T::line_coord (main_) + pos);
      pos += T::line_size (p) + padding_;
    }

    widget_vector widgets_;             /**< widgets to pack */
    panel   *main_;             /**< main widget, we will try to place all widgets under it*/
    int padding_;                /**< size of empty area between widgets on the line */
    align::type align_;             /**< aligment of the widgets */
  };

  typedef packer<pack::horizontal> horizontal_packer;
  typedef packer<pack::vertical> vertical_packer;
}

#endif
