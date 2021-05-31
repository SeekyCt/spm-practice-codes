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
    virtual void close() override;

public:
    GameSaveMenu();
};

}
