/*
    GameSaveMenu
    Controls for saving & loading the game save file
*/

#pragma once

#include "mod_ui_base/menuwindow.h"

namespace mod {

#define MAIN_MENU_OPTION_COUNT 9

class GameSaveMenu : public MenuWindow
{
private:
    virtual void close() override;
public:
    GameSaveMenu();
};

}
