/*
    MapSelectMenu
    Menu window to change the current map
*/

#pragma once

#include <common.h>

#include "mod_ui_base/centredbutton.h"
#include "mod_ui_base/menuicon.h"
#include "mod_ui_base/menuscrollgroup.h"
#include "childmenu.h"

namespace mod {

class InventoryMenu final : public ChildMenu
{
private:
    // Common
    int mScreen;
    int mSlot;
    int mMode;
    int getId(int slot);
    
    // Slot selection screen
    MenuButton * mModeButtons[5];
    MenuIcon * mItemButtons[32];
    int getCount();
    int getMax();
    static bool modeChange(MenuButton * button, void * param);
    static bool chooseSlot(MenuButton * button, void * param);
    static bool addItem(MenuButton * button, void * param);
    void updateItemDisps();
    void initSelectScreen();
    void exitSelectScreen();

    // Edit mode selection screen
    bool mShowSelectable;
    MenuButton * mEditButtons[3];
    bool getSelectable();
    void setSelectable(bool selectable);
    static bool chooseChange(MenuButton * button, void * param);
    static bool chooseDelete(MenuButton * button, void * param);
    static bool toggleSelectable(MenuButton * button, void * param);
    void updateSelectableDisp();
    void initEditScreen();
    void exitEditScreen();
    
    // Item id edit screen
    MenuScrollGroup * mIdScroller;
    MenuIcon * mItemIcon;
    CentredButton * mItemName;
    void setId(int id);
    static void idChange(MenuScrollGroup * scroller, s32 delta, void * param);
    static bool finishId(MenuButton * button, void * param);
    void updateIdDisp();
    void initIdScreen();
    void exitIdScreen();

protected:
    virtual void close() override;

public:
    InventoryMenu();
};

}
