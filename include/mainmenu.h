/*
    MainMenu
    Root menu window
*/

#pragma once

#include "mod_ui_base/menuwindow.h"

namespace mod {

#define MAIN_MENU_OPTION_COUNT 11

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
