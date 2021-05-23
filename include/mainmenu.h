/*
    MainMenu
    Root menu window
*/

#pragma once

#include "mod_ui_base/menuwindow.h"

namespace mod {

#define MAIN_MENU_OPTION_COUNT 9

class MainMenu : public MenuWindow
{
private:
    MenuButton * mOptions[MAIN_MENU_OPTION_COUNT];

public:
    MainMenu();
};

}
