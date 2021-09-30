#include "scriptvarmenu.h"
#include "util.h"

#include <types.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <wii/stdio.h>

namespace mod {

enum
{
    TYPE_BYTE,
    TYPE_WORD,
    TYPE_FLAG
};

struct VarGroup
{
    char name[5];
    u8 type;
    u16 count;
};

enum
{
    GW  = 0, GF   = 1,
    GSW = 2, GSWF = 3,
    LSW = 4, LSWF = 5
};

static VarGroup groups[] {
    {"GW",  TYPE_WORD, 32},   {"GF",   TYPE_FLAG, 96},
    {"GSW", TYPE_BYTE, 2048}, {"GSWF", TYPE_FLAG, 8192},
    {"LSW", TYPE_BYTE, 1024}, {"LSWF", TYPE_FLAG, 512}
};

void ScriptVarMenu::updateGroupDisp()
{
    // Change message to current group name
    mGroupDisp->mMsg = groups[mGroup].name;
}

void ScriptVarMenu::updateVarDisp()
{
    // Display current id and value
    mSelectScroller->mDispValue = mId;
    mEditScroller->mDispValue = getVal();
}

u32 ScriptVarMenu::getVal()
{
    switch (mGroup)
    {
        case GW:
            return EVT_GET_GW(mId);
        case GF:
            return EVT_GET_GF(mId);
        case GSW:
            return spm::swdrv::swByteGet(mId);
        case GSWF:
            return spm::swdrv::swGet(mId);
        case LSW:
            return spm::swdrv::_swByteGet(mId);
        case LSWF:
            return spm::swdrv::_swGet(mId);
        default:
            return 0; // never reached but stops compiler complaining
    }
}

void ScriptVarMenu::setVal(u32 val)
{
    // No invalid values should make it here but they're asserted
    // just in case to prevent save corruption risks
    switch (mGroup)
    {
        case GW:
            EVT_SET_GW(mId, val);
            break;
        case GF:
            assertf(IS_BOOL(val), "ScriptVarMenu::setVal invalid GF 0x%x", val);
            if (val)
                EVT_SET_GF(mId);
            else
                EVT_CLEAR_GF(mId);
            break;
        case GSW:
            assertf(IS_BYTE(val) || (mId == 0), "ScriptVarMenu::setVal invalid GSW 0x%x", val);
            spm::swdrv::swByteSet(mId, val);
            break;
        case GSWF:
            assertf(IS_BOOL(val), "ScriptVarMenu::setVal invalid GSWF 0x%x", val);
            if (val)
                spm::swdrv::swSet(mId);
            else
                spm::swdrv::swClear(mId);
            break;
        case LSW:
            assertf(IS_BYTE(val), "ScriptVarMenu::setVal invalid LSW 0x%x", val);
            spm::swdrv::_swByteSet(mId, val);
            break;
        case LSWF:
            assertf(IS_BOOL(val), "ScriptVarMenu::setVal invalid LSWF 0x%x", val);
            if (val)
                spm::swdrv::_swSet(mId);
            else
                spm::swdrv::_swClear(mId);
            break;
    }
}

void ScriptVarMenu::groupUp(MenuScroller * scroller, void * param)
{
    (void) scroller;

    // Decrement group and reset selected
    ScriptVarMenu * instance = reinterpret_cast<ScriptVarMenu *>(param);
    instance->mGroup -= 1;
    instance->mId = 0;

    // Loop around if the end of the group list is reached
    if (instance->mGroup < 0)
        instance->mGroup = ARRAY_SIZEOF(groups) - 1;

    // Process new value
    instance->updateGroupDisp();
    instance->updateVarDisp();
}

void ScriptVarMenu::groupDown(MenuScroller * scroller, void * param)
{
    (void) scroller;

    // Increment group and reset selected
    ScriptVarMenu * instance = reinterpret_cast<ScriptVarMenu *>(param);
    instance->mGroup += 1;
    instance->mId = 0;

    // Loop around if the end of the group list is reached
    if (instance->mGroup >= (int) ARRAY_SIZEOF(groups))
        instance->mGroup = 0;
    
    // Process new value
    instance->updateGroupDisp();
    instance->updateVarDisp();
}

void ScriptVarMenu::idChange(MenuScrollGroup * scroller, s32 delta, void * param)
{
    (void) scroller;
    ScriptVarMenu * instance = reinterpret_cast<ScriptVarMenu *>(param);

    // Update value
    instance->mId += delta;

    // Set value with prevention of over/underflow
    if (instance->mId >= groups[instance->mGroup].count)
        instance->mId = groups[instance->mGroup].count - 1;
    if (instance->mId < 0)
        instance->mId = 0;

    // Update display
    instance->updateVarDisp();
}

void ScriptVarMenu::valChange(MenuScrollGroup * scroller, s32 delta, void * param)
{
    (void) scroller;

    ScriptVarMenu * instance = reinterpret_cast<ScriptVarMenu *>(param);

    // Find maximum value
    s64 max;
    u8 type = groups[instance->mGroup].type;
    if ((type == TYPE_WORD) || ((instance->mId == 0) && (instance->mGroup == GSW)))
        max = 0xffffffff;
    else if (type == TYPE_BYTE)
        max = 0xff;
    else
        max = 1;

    // Set value with prevention of overflow
    s64 val = instance->getVal();
    if ((val + delta) > max)
        val = max;
    else if ((val + delta) < 0)
        val = 0;
    else
        val = val + delta;
    instance->setVal(val);

    // Update display
    instance->updateVarDisp();
}

void ScriptVarMenu::close()
{
    // Re-enable script variable logging
    scriptLogOnOff(true);

    // Close as normal
    MenuWindow::close();
}

void ScriptVarMenu::fullClose()
{
    // Re-enable script variable logging
    scriptLogOnOff(true);

    // Close as normal
    ChildMenu::fullClose();
}

ScriptVarMenu::ScriptVarMenu()
{
    // Init selection
    mGroup = GW;
    mId = 0;

    // Position constants
    const f32 groupDispX = -200.0f;
    const f32 selectDispX = groupDispX + 85.0f;
    const f32 middleTextX = selectDispX + (FONT_WIDTH * 4.5f);
    const f32 editDispX = middleTextX + (FONT_WIDTH * 3.0f);
    const f32 dispsY = 20.0f;

    // Init group select buttons
    mGroupDisp = new MenuScroller(this, groups[mGroup].name, groupDispX, dispsY, 20.0f, groupUp, groupDown, this);

    // Init id select button
    mSelectScroller = new MenuScrollGroup(this, mId, selectDispX, dispsY, idChange, this, 4, false);
    buttonLinkHorizontal(mGroupDisp, mSelectScroller);

    // Init value select button
    mEditScroller = new MenuScrollGroup(this, getVal(), editDispX, dispsY, valChange, this, 8, true);   
    buttonLinkHorizontal(mSelectScroller, mEditScroller);

    // Create cosmetic '= 0x' display
    MenuButton * middleText = new MenuButton(this, "= 0x", middleTextX, dispsY);
    (void) middleText;

    // Disable script variable logging while open
    scriptLogOnOff(false);

    // Set starting button and title
    mCurButton = mGroupDisp;
    mTitle = "Script Variable Editor";
}

}
