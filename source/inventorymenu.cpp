#include "inventorymenu.h"
#include "mainmenu.h"
#include "util.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/item_data.h>
#include <spm/mario_pouch.h>
#include <spm/msgdrv.h>
#include <wii/stdio.h>

namespace mod {

/*
    Common
*/

enum
{
    SCREEN_SELECT = 0,
    SCREEN_EDIT = 1,
    SCREEN_ID = 2
};

enum
{
    MODE_KEY_ITEM = 0,
    MODE_USE_ITEM = 1,
    MODE_STORED = 2,
    MODE_CHARS = 3,
    MODE_PIXLS = 4
};

struct ModeDef
{
    int min; // inclusive
    int max; // exclusive
    int slotCount;
    const char * name;
};

ModeDef modes[]
{
/* MODE_KEY_ITEM */ { 16,  65, 32,    "Key Items"},
/* MODE_USE_ITEM */ { 65, 216, 10, "Usable Items"},
/* MODE_STORED   */ { 65, 216, 32, "Stored Items"},
/* MODE_CHARS    */ {216, 220,  4,   "Characters"},
/* MODE_PIXLS    */ {220, 232, 16,        "Pixls"} 
};

int InventoryMenu::getId(int slot)
{
    spm::mario_pouch::MarioPouchWork * pp = spm::mario_pouch::pouchGetPtr();
    switch (mMode)
    {
        case MODE_KEY_ITEM:
            return pp->keyItem[slot];
        case MODE_USE_ITEM:
            return pp->useItem[slot];
        case MODE_STORED:
            return pp->shopItem[slot];
        case MODE_CHARS:
            return pp->characters[slot].itemType;
        case MODE_PIXLS:
            return pp->pixls[slot].itemType;
        default:
            return 0;
    }
}

/*
    Slot selection screen
*/

int InventoryMenu::getCount()
{
    switch (mMode)
    {
        case MODE_KEY_ITEM:
            return spm::mario_pouch::pouchCountKeyItems();
        case MODE_USE_ITEM:
            return spm::mario_pouch::pouchCountUseItems();
        case MODE_STORED:
            return spm::mario_pouch::pouchCountShopItems();
        case MODE_CHARS:
            return spm::mario_pouch::pouchCountChars();
        case MODE_PIXLS:
            return spm::mario_pouch::pouchCountPixls();
        default:
            return 0;
    }
}

int InventoryMenu::getMax()
{
    switch (mMode)
    {
        case MODE_KEY_ITEM:
            return 32;
        case MODE_USE_ITEM:
            return 10;
        case MODE_STORED:
            return 32;
        case MODE_CHARS:
            return 4;
        case MODE_PIXLS:
            return 16;
        default:
            return 0;
    }
}

bool InventoryMenu::modeChange(MenuButton * button, void * param)
{
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);

    // Figure out which button is being hovered over
    int i;
    for (i = 0; i < 5; i++)
        if (button == instance->mModeButtons[i]) break;
    
    // If the cursor has moved, update for the new mode
    if (instance->mMode != i)
    {
        instance->mMode = i;

        instance->updateItemDisps();
    }

    return true;
}

bool InventoryMenu::chooseSlot(MenuButton * button, void * param)
{
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);
    int i;
    for (i = 0; i < 32; i++)
        if (instance->mItemButtons[i] == button)
            break;

    instance->mSlot = i;
    instance->exitSelectScreen();
    instance->initEditScreen();
    return false;
}

bool InventoryMenu::addItem(MenuButton * button, void * param)
{
    (void) button;
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);

    instance->mSlot = instance->getCount();
    instance->setId(modes[instance->mMode].min);
    instance->setSelectable(true); // will just be ignored if not needed
    instance->exitSelectScreen();
    instance->initIdScreen();
    return false;
}

void InventoryMenu::updateItemDisps()
{
    // Wipe buttons
    for (int i = 0; i < 32; i++)
    {
        if (mItemButtons[i] == nullptr)
            break;
        delete mItemButtons[i];
        mItemButtons[i] = nullptr;
    }

    const f32 firstX = -115.0f;
    const f32 firstY = 150.0f;
    const f32 diff = 50.0f;

    int n = getCount();

    // Create new buttons
    for (int i = 0; i < n; i++)
    {
        spm::item_data::ItemData * item = spm::item_data::itemDataTable + getId(i);
        mItemButtons[i] = new MenuIcon(this, item->iconId, firstX + (diff * (i % 8)), firstY - (diff * (i / 8)), chooseSlot, this);
    }
    if (n != getMax())
        mItemButtons[n] = new MenuIcon(this, 176, firstX + (diff * (n % 8)), firstY - (diff * (n / 8)), addItem, this);

    // Horizontal link
    for (int row = 0; row < 32; row += 8)
    {
        int col;
        for (col = 0; col < 7; col++)
        {
            if (mItemButtons[row + col + 1] == nullptr)
                break;
            buttonLinkHorizontal(mItemButtons[row + col], mItemButtons[row + col + 1]);
        }
        if (col < 7) // if breaking from the other loop, break here too
            break;
    }

    // Vertical link
    for (int col = 0; col < 8; col++)
    {
        for (int row = 0; row < 24; row += 8)
        {
            if (mItemButtons[row + col + 8] == nullptr)
                break;
            buttonLinkVertical(mItemButtons[row + col], mItemButtons[row + col + 8]);
        }
        // This needs to run for every column even if one ends early
    }

    // Exit link
    for (int row = 0; row < 32; row += 8)
    {
        if (mItemButtons[row] == nullptr)
            break;
        buttonLinkHorizontal(mModeButtons[mMode], mItemButtons[row]);
    }
    buttonLinkHorizontal(mModeButtons[mMode], mItemButtons[0]);
}

void InventoryMenu::initSelectScreen()
{
    mScreen = SCREEN_SELECT;

    // Position constants
    const f32 modeX = -330.0f;
    const f32 firstModeY = 100.0f;

    // Init mode select buttons
    for (int i = 0; i < 5; i++)
        mModeButtons[i] = new MenuButton(this, modes[i].name, modeX, firstModeY - (i * (FONT_HEIGHT + 5)), modeChange, this, true);
    for (int i = 0; i < 4; i++)
        buttonLinkVertical(mModeButtons[i], mModeButtons[i + 1]);

    updateItemDisps();
    buttonLinkHorizontal(mModeButtons[mMode], mItemButtons[0]);
    if (mSlot == -1)
        mCurButton = mModeButtons[mMode];
    else
        mCurButton = mItemButtons[mSlot];
}

void InventoryMenu::exitSelectScreen()
{
    for (int i = 0; i < 5; i++)
    {
        delete mModeButtons[i];
        mModeButtons[i] = nullptr;
    }
    for (int i = 0; i < 32; i++)
    {
        if (mItemButtons[i] == nullptr)
            break;
        
        delete mItemButtons[i];
        mItemButtons[i] = nullptr;
    }
}

/*
    Item edit screen
*/

bool InventoryMenu::getSelectable()
{
    spm::mario_pouch::MarioPouchWork * pp = spm::mario_pouch::pouchGetPtr();
    switch (mMode)
    {
        case MODE_CHARS:
            return pp->characters[mSlot].selectable;
        case MODE_PIXLS:
            return pp->pixls[mSlot].selectable;
        default:
            return false;
    }
}

void InventoryMenu::setSelectable(bool selectable)
{
    spm::mario_pouch::MarioPouchWork * pp = spm::mario_pouch::pouchGetPtr();
    switch (mMode)
    {
        case MODE_CHARS:
            pp->characters[mSlot].selectable = selectable;
            break;
        case MODE_PIXLS:
            pp->pixls[mSlot].selectable = selectable;
            break;
        default:
            break;
    }
}

bool InventoryMenu::chooseChange(MenuButton * button, void * param)
{
    (void) button;
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);

    instance->exitEditScreen();
    instance->initIdScreen();
    return false;
}

bool InventoryMenu::chooseDelete(MenuButton * button, void * param)
{
    (void) button;
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);

    if (instance->mMode == MODE_CHARS)
    {
        // pouchRemoveItem doesn't support characters
        spm::mario_pouch::MarioPouchWork * pp = spm::mario_pouch::pouchGetPtr();
        for (int i = instance->mSlot; i < 4; i++)
            pp->characters[i] = pp->characters[i + 1];
        pp->characters[3].itemType = 0;
        pp->characters[3].selectable = false;
        pp->characters[3].selected = false;
    }
    else if (instance->mMode == MODE_STORED)
    {
        // pouchRemoveItem doesn't support shopItem
        spm::mario_pouch::pouchRemoveShopItem(instance->getId(instance->mSlot));
    }
    else
    {
        spm::mario_pouch::pouchRemoveItem(instance->getId(instance->mSlot));
    }
    instance->mSlot = 0;
    instance->exitEditScreen();
    instance->initSelectScreen();
    return false;
}

bool InventoryMenu::toggleSelectable(MenuButton * button, void * param)
{
    (void) button;
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);

    instance->setSelectable(!instance->getSelectable());
    instance->updateSelectableDisp();
    return true;
}

void InventoryMenu::updateSelectableDisp()
{
    if (getSelectable())
        mEditButtons[2]->mMsg = "Selectable: on";
    else
        mEditButtons[2]->mMsg = "Selectable: off";
}

void InventoryMenu::initEditScreen()
{
    mScreen = SCREEN_EDIT;
    mShowSelectable = (mMode == MODE_PIXLS) || (mMode == MODE_CHARS);

    const f32 x = -100.0f;
    const f32 firstY = 50.0f;
    const f32 yDiff = FONT_HEIGHT + 5;

    mEditButtons[0] = new MenuButton(this, "Change Item", x, firstY, chooseChange, this);
    mEditButtons[1] = new MenuButton(this, "Delete item", x, firstY - yDiff, chooseDelete, this);
    buttonLinkVertical(mEditButtons[0], mEditButtons[1]);
    if (mShowSelectable)
    {
        mEditButtons[2] = new MenuButton(this, nullptr, x, firstY - (yDiff * 2), toggleSelectable, this);
        buttonLinkVertical(mEditButtons[1], mEditButtons[2]);
        updateSelectableDisp();
    }

    mCurButton = mEditButtons[0];
}

void InventoryMenu::exitEditScreen()
{
    int n = mShowSelectable ? 3 : 2;
    for (int i = 0; i < n; i++)
    {
        delete mEditButtons[i];
        mEditButtons[i] = nullptr;
    }
}

/*
    Id screen
*/

void InventoryMenu::setId(int id)
{
    spm::mario_pouch::MarioPouchWork * pp = spm::mario_pouch::pouchGetPtr();
    switch (mMode)
    {
        case MODE_KEY_ITEM:
            pp->keyItem[mSlot] = id;
            break;
        case MODE_USE_ITEM:
            pp->useItem[mSlot] = id;
            break;
        case MODE_STORED:
            pp->shopItem[mSlot] = id;
            break;
        case MODE_CHARS:
            pp->characters[mSlot].itemType = id;
            break;
        case MODE_PIXLS:
            pp->pixls[mSlot].itemType = id;
            break;
        default:
            break;
    }
}

void InventoryMenu::idUp(MenuScroller * scroller, void * param)
{
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);

    // Figure out which digit is being changed and its place value
    s32 increment = 100;
    for (int i = 0; i < 3; i++)
    {
        if (scroller == instance->mIdScrollers[i])
            // Use current increment and exit loop
            break;
        else
            // Each digit has a value 10 times lower than the previous
            increment /= 10;
    }

    // Set value
    int id = instance->getId(instance->mSlot) + increment;

    if (id >= modes[instance->mMode].max)
        id = modes[instance->mMode].max - 1;

    instance->setId(id);

    // Update display
    instance->updateIdDisp();
}

void InventoryMenu::idDown(MenuScroller * scroller, void * param)
{
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);

    // Figure out which digit is being changed and its place value
    s32 increment = 100;
    for (int i = 0; i < 3; i++)
    {
        if (scroller == instance->mIdScrollers[i])
            // Use current increment and exit loop
            break;
        else
            // Each digit has a value 10 times lower than the previous
            increment /= 10;
    }

    // Update value
    int id = instance->getId(instance->mSlot) - increment;

    if (id < modes[instance->mMode].min)
        id = modes[instance->mMode].min;

    instance->setId(id);

    // Update display
    instance->updateIdDisp();
}

void InventoryMenu::updateIdDisp()
{
    // Set select display to each individual decimal digit of id
    int id = getId(mSlot);
    spm::item_data::ItemData * item = spm::item_data::itemDataTable + id;
    mItemIcon->mIconId = item->iconId;

    if (id != 0)
        mItemName->mMsg = spm::msgdrv::msgSearch(item->nameMsg);
    else
        mItemName->mMsg = "None";

    wii::stdio::sprintf(mIdStrs[0], "%d", DIGIT_100(id));
    wii::stdio::sprintf(mIdStrs[1], "%d", DIGIT_10(id));
    wii::stdio::sprintf(mIdStrs[2], "%d", DIGIT_1(id));
}

bool InventoryMenu::finishId(MenuButton * button, void * param)
{
    InventoryMenu * instance = reinterpret_cast<InventoryMenu *>(param);
    instance->close();
    return false;
}

void InventoryMenu::initIdScreen()
{    
    mScreen = SCREEN_ID;
    const f32 selectDispX = -60.0f;
    const f32 selectXDiff = 20.0f;
    const f32 dispsY = 20.0f;
    const f32 iconX = selectDispX + 80.0f;
    const f32 iconY = dispsY + 15.0f;
    const f32 nameY = -100.0f;
    
    // Init id select buttons
    mItemIcon = new MenuIcon(this, 0, iconX, iconY);
    mItemName = new CentredButton(this, nullptr, nameY);
    updateIdDisp();
    for (int i = 0; i < 3; i++)
        mIdScrollers[i] = new MenuScroller(this, mIdStrs[i], selectDispX + (selectXDiff * i), dispsY,
                                           0.0f, idUp, idDown, this, finishId, this);
    for (int i = 0; i < 2; i++)
        buttonLinkHorizontal(mIdScrollers[i], mIdScrollers[i + 1]);
    mCurButton = mIdScrollers[0];
}

void InventoryMenu::exitIdScreen()
{
    for (int i = 0; i < 3; i++)
    {
        delete mIdScrollers[i];
        mIdScrollers[i] = nullptr;
    }
    delete mItemIcon;
    delete mItemName;
    mItemIcon = nullptr;
    mItemName = nullptr;
}

/*
    Other
*/

void InventoryMenu::close()
{
    if (mScreen == SCREEN_ID)
    {
        exitIdScreen();
        initSelectScreen();
    }
    else if (mScreen == SCREEN_EDIT)
    {
        exitEditScreen();
        initSelectScreen();
    }
    else
    {
        // Change back to parent menu
        delete MenuWindow::sCurMenu;
        MenuWindow::sCurMenu = new MainMenu();
    }
}

InventoryMenu::InventoryMenu()
{
    mMode = MODE_KEY_ITEM;
    mTitle = "Inventory Editor";
    mSlot = -1;

    initSelectScreen();
}

}
