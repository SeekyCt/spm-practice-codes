/*
    MainMenu
    Root menu window
*/

#pragma once

#include <common.h>

#include "mod_ui_base/menuwindow.h"

namespace mod {

#define MAIN_MENU_OPTION_COUNT 13

class MainMenu final : public MenuWindow
{
private:
    MenuButton * mOptions[MAIN_MENU_OPTION_COUNT];
    static s32 sLastOption;
    
    void recordLastOption(); 

public:
    MainMenu();
    virtual ~MainMenu();
};

}
