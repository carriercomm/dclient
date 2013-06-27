/* button.hh
 * This file is part of ndk library
 * Copyright (c) 2003,2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libndk as the file COPYING.
 */
#ifndef NDK_XX_BUTTON_H
#define NDK_XX_BUTTON_H

#include <ndk/label.hh>

namespace ndk
{
  /**
   * Simple version of the pre-button without activation/deactivation feature.
   * This button only emits 'clicked' signal
   * this class between label and button now to prevent dublication of on_click code
   */
  struct mini_button
    : label
  {
    mini_button (const std::string &str, panel *parent = 0, bool accept = true);
  };


  /**
   * Pre-Button: Label with activation support
   * This is a base class for Radio/Check/Push buttons
   */
  struct button
    : label
  {
    /**
     * @see NDK::Label() for details
     * @param accept additional parameter, accept or not focus
     * buttons without focus acception are usefull for widgets like ListBox, SwitchButton
     */
    button (const std::string &str, panel *parent = 0, bool accept = true);

  protected:
    virtual void set_color (pen &p);           /** set default color of the label */
    int state_;             /**< last status */
    bool selected_;                /**< semaphore: are this object selected now or not */

  private:
    event::result at_system (const event &);           /**< system event processor */
  };
}

#endif
