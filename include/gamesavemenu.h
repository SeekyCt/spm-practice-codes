/*
    GameSaveMenu
    Controls for saving & loading the game save file
*/

#pragma once

#include "mod_ui_base/menuwindow.h"

namespace mod {

class GameSaveMenu : public MenuWindow
{
private:
    MenuButton * mButtons[2];

    virtual void close() override;
    void initMainScreen();
    void initConfirmScreen();
    void exitScreen();
    static bool openMainScreen(MenuButton * button, void * param);
    static bool openConfirmScreen(MenuButton * button, void * param);

public:
    GameSaveMenu();

    static void pitSavePatch();
    
};

}
