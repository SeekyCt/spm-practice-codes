/*
    NandMenu
    Controls for saving & loading settings from NAND
*/

#pragma once

#include "mod_ui_base/menuwindow.h"

namespace mod {

enum class NandTask : u8
{
    NONE,
    LOAD,
    SAVE,
    DELETE
};

class NandMenu : public MenuWindow
{
private:
    s32 mEvtId;
    const char * mMiddleMsg;
    const char * mDoneMsg;
    MenuButton * mButtonPtrs[4];
    NandTask mTask;
    u8 mCurButtonIdx;
    bool mShouldClose;

    void spawnButtons();
    void removeButtons();

    static bool loadSettings(MenuButton * button, void * param);
    static bool saveSettings(MenuButton * button, void * param);
    static bool defaultSettings(MenuButton * button, void * param);
    static bool deleteSettings(MenuButton * button, void * param);

protected:
    virtual void close() override;
    virtual void disp() override;

public:
    NandMenu();
};

}
