/* edit.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#ifndef NDK_EDIT_HH__
#define NDK_EDIT_HH__

#include <vector>

#include <ndk/button.hh>

namespace ndk
{
  /**
   *  Line Edit widget.
   *  This widget don't made auto resize at draw() as Label or Button,
   *  so you need to set widget size itself using resize()
   *  This widgets inherits from button, 'cause we can insert this widget to the list,
   *  and we need activation/deactivation feature to show/hide cursor
   */
  class edit
    : public button
  {
  public:
    enum echo_mode
    {
      /**< possible draw modes of the edit*/
      normal,                               /**<  display characters as they are entered */
      password,                               /**< display asterisks instead of the characters actually entered */
      no_echo                   /**<  do not display anything */
    };

    edit (const std::string &str = "", panel *parent = 0, bool accept = true);
    void resize (int size);            /**<  * helper to resize widget only in one direction */

    /* some edit properties */
    void set_text (const std::string &str);            /**<set text, draw and reset current cursor position */
    void set_mode (echo_mode mode);            /**< set edit's text display mode */

    std::function<void (std::string)> &on_commit () { return on_commit_; }

  protected:
    event::result at_keyboard (const keyboard &ev);
    virtual void change_color ();

  private:
    virtual void draw ();            /**< special draw method for edit */

    std::function<void (std::string)> on_commit_;

    event::result at_system (const event &ev);

    bool scroll_right ();            /**< scroll viewport to the right */
    bool scroll_left ();            /**< scroll viewport to the left */
    bool recalibrate ();            /**< recalibrate begin_ position */
    void set_cursor ();            /**< move cursor to current position */
    void insert (int pos, char sym);            /**< insert/replace symbol at position */
    void erase (int pos);            /**< erase/replace symbol at position */

    size_t current_;                /**< current cursor position under string */
    size_t begin_;                /**< beginning of the string */
    echo_mode mode_;
  };

  class input
    : public panel
  {
    /**< This is a line edit with label */
  public:
    input (const std::string &label, panel *parent = 0);
    void resize (int size);

    void set_text (const std::string &str);
    void set_label (const std::string &str);
    void set_mode (edit::echo_mode mode);

    const std::string &text () const;

    std::function<void (std::string)> &on_commit () { return edit_.on_commit (); }

  private:
    virtual void draw ();

    event::result at_system (const event &ev);

    label label_;
    edit edit_;
  };
}
#endif
