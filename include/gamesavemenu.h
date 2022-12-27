/*
    GameSaveMenu
    Controls for saving & loading the game save file
*/

#pragma once

#include <common.h>

#include "childmenu.h"

namespace mod {

class GameSaveMenu final : public ChildMenu
{
private:
    MenuButton * mButtons[2];

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
