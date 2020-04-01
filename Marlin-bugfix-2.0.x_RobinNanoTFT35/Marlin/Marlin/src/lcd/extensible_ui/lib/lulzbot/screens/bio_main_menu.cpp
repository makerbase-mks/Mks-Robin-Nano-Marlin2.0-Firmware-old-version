/*********************
 * bio_main_menu.cpp *
 *********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI) && defined(LULZBOT_USE_BIOPRINTER_UI)

#include "screens.h"

using namespace FTDI;
using namespace Theme;

void MainMenu::onRedraw(draw_mode_t what) {
  #define GRID_ROWS 10
  #define GRID_COLS 2

  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .font(font_large).text( BTN_POS(1,1), BTN_SIZE(2,1), GET_TEXT_F(MAIN_MENU))
       .colors(normal_btn)
       .font(font_medium)
       .tag(2).button( BTN_POS(1,2), BTN_SIZE(2,1), GET_TEXT_F(MOVE_TO_HOME))
       .tag(3).button( BTN_POS(1,3), BTN_SIZE(2,1), GET_TEXT_F(RAISE_PLUNGER))
       .tag(4).button( BTN_POS(1,4), BTN_SIZE(2,1), GET_TEXT_F(RELEASE_XY_AXIS))
       .tag(5).button( BTN_POS(1,5), BTN_SIZE(2,1), GET_TEXT_F(AUTOLEVEL_X_AXIS))
       .tag(6).button( BTN_POS(1,6), BTN_SIZE(2,1), GET_TEXT_F(BED_TEMPERATURE))
       .tag(7).button( BTN_POS(1,7), BTN_SIZE(2,1), GET_TEXT_F(INTERFACE_SETTINGS))
       .tag(8).button( BTN_POS(1,8), BTN_SIZE(2,1), GET_TEXT_F(ADVANCED_SETTINGS))
       .tag(9).button( BTN_POS(1,9), BTN_SIZE(2,1), GET_TEXT_F(ABOUT_PRINTER))
       .colors(action_btn)
       .tag(1).button( BTN_POS(1,10), BTN_SIZE(2,1), GET_TEXT_F(BACK));
  }

  #undef GRID_COLS
  #undef GRID_ROWS
}

bool MainMenu::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  const bool e_homed = isAxisPositionKnown(E0);

  switch (tag) {
    case 1: SaveSettingsDialogBox::promptToSaveSettings();                               break;
    case 2: GOTO_SCREEN(BioConfirmHomeXYZ);                                              break;
    case 3: SpinnerDialogBox::enqueueAndWait_P(e_homed ? F("G0 E0 F120") : F("G112"));   break;
    case 4: StatusScreen::unlockMotors();                                                break;
    case 5: SpinnerDialogBox::enqueueAndWait_P(F(LULZBOT_AXIS_LEVELING_COMMANDS));       break;
    case 6: GOTO_SCREEN(TemperatureScreen);                                              break;
    case 7: GOTO_SCREEN(InterfaceSettingsScreen);                                        break;
    case 8: GOTO_SCREEN(AdvancedSettingsMenu);                                           break;
    case 9: GOTO_SCREEN(AboutScreen);                                                    break;
    default:
      return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI
