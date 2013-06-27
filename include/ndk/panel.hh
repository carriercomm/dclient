/* panel.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#pragma once

#include <ndk/transport.hh>
#include <ndk/window.hh>

#include <curses.h>
#include <panel.h>

#include <vector>

namespace ndk
{
  /**
   * meaning of coordinates
   */
  enum coords_type
  {
    absolute,                   /**< absolute coordinate of screen */
    relative,                   /**< relative to previous position coordinate */
    rel_parent                  /**< relative to parent coordinate */
  };

  /**
   * Extended version of panel, supports all NDK event engine
   * and general color pallete
   */
  struct panel
    : window
  {
    typedef std::vector<panel *> child_vector;            /**< list of the panels */

    explicit panel (panel *parent = 0);
    virtual ~panel ();

    virtual void top ();            /**< put this panel on top of all panels in the  stack */
    virtual void bottom ();            /**< put this panel at the bottom of all panels */
    virtual void show ();            /**<  makes  a hidden panel visible by placing it on top of the panels */
    virtual void hide ();            /**< removes  the  given panel from the panel stack and thus hides it from view */

    bool    hidden () const;            /**< are this widget hidden? */
    bool    higher (const panel &) const;           /**< are we higher other panel */


    virtual void draw ();            /**< draw widget specific content over general panel */
    void prepare (bool recursive = true);            /**< draw this widget and all subwidgets */
    void visualize (bool recursive = true);            /**< equivalent of prepare + show */

    /**
     * move panel to the other place, refresh state of other panels
     * @param x absolute or relative to previous position x coordinate
     * @param y absolute or relative to previous position y coordinate
     * @param
     */
    virtual void move (int x, int y, coords_type type = relative);

    panel *parent () const { return parent_; }

    /**
     * return padding size from the edge, helps to control drawing area
     * it's usefull for packing under frames: active area will be
     * (x()+padding(),x()+width()-padding()) x (y()+padding(),y()+height()-padding())
     */
    virtual int padding () const;

    void emit (int);                    // gate to emit some events
    engine *events ();                  // entry point to events roadway
    void in_focus (panel &);            // set widget as in focus

    void focus ();

  protected:
    void    add_child (panel *p);            /**< helper to create panel's hieracly */
    void    del_child (panel *p);            /**< delete from the childs list */

  private:
    /* hide dangerous operations */
    panel (const panel &other);
    panel &operator = (const panel &other);

    PANEL *panel_;             /**< internal ncurses panel structure */
    panel *parent_;             /**< parent panel, if we have*/
    child_vector childs_;             /**< childs of this */

  private:
    event::result at_system (const event &);
    engine events_;
  };

  static inline bool
  operator < (const panel &lhs, const panel &rhs)
  {
    return rhs.higher (lhs);
  }

  static inline bool
  operator > (const panel &lhs, const panel &rhs)
  {
    return lhs.higher (rhs);
  }
}
