#pragma once

#include <types.h>
#include <spm/evtmgr.h>
#include <spm/filemgr.h>
#include <wii/types.h>

namespace spm::pausewin {

#define PAUSE_FLAG_HIGHLIGHT 0x10
#define PAUSE_FLAG_OSCILLATE 0x100

enum PauseTextureId
{
    PAUSETEX_NONE = 0, // use with boxOnly
    PAUSETEX_CHARACTERS_BTN = 0,
    PAUSETEX_PIXLS_BTN = 1,
    PAUSETEX_ITEMS_BTN = 2,
    PAUSETEX_KEY_ITEMS_BTN = 3,
    PAUSETEX_CHAPTERS_BTN = 4,
    PAUSETEX_CARDS_BTN = 5,
    PAUSETEX_RECIPES_BTN = 6,
    PAUSETEX_MAPS_BTN = 7,
    PAUSETEX_CHAPTER_1_BTN = 8,
    PAUSETEX_CHAPTER_2_BTN = 9,
    PAUSETEX_CHAPTER_3_BTN = 10,
    PAUSETEX_CHAPTER_4_BTN = 11,
    PAUSETEX_CHAPTER_5_BTN = 12,
    PAUSETEX_CHAPTER_6_BTN = 13,
    PAUSETEX_CHAPTER_7_BTN = 14,
    PAUSETEX_CHAPTER_8_BTN = 15,
    // ...
};

enum PlusWinButtonIdx
{
    PLUSWIN_BTN_CHARS = 0,
    PLUSWIN_BTN_PIXLS = 1,
    PLUSWIN_BTN_ITEMS = 2,
    PLUSWIN_BTN_KEY_ITEMS = 3,
    PLUSWIN_BTN_CHAPTERS = 4,
    PLUSWIN_BTN_CARDS = 5,
    PLUSWIN_BTN_RECIPES = 6,
    PLUSWIN_BTN_MAPS = 7,
    PLUSWIN_BTN_BG = 8,
    PLUSWIN_BTN_HELP = 9,
    PLUSWIN_BTN_STATS = 10
};

enum PluswinState
{
    PLUSWIN_STATE_MAIN_MENU = 0,
    PLUSWIN_STATE_KEY_ITEMS = 10,
    PLUSWIN_STATE_CHAPTERS = 20,
    PLUSWIN_STATE_CARDS = 30,
    PLUSWIN_STATE_RECIPES = 40,
    PLUSWIN_STATE_MAPS = 50,
};

typedef void (PausewinFunc)(struct PausewinEntry *);

struct PausewinEntry
{
    u16 flags;
    u16 id;
    wii::Vec2 pos;
    f32 width;
    f32 height;
    wii::Vec2 moveStart;
    wii::Vec2 moveDest;
    wii::Vec2 originalPos;
    f32 scale;
    u8 unknown_0x30[0x34 - 0x30];
    s32 alpha;
    s32 stateProgress;
    s32 state;
    s32 boxOnly;
    s32 textureId;
    s32 priority;
    s32 curMsgLine;
    s32 msgWidth;
    const char * msg;
    u8 unknown_0x58[0x64 - 0x58];
    PausewinFunc * initFunc;
    PausewinFunc * mainFunc;
    PausewinFunc * dispFunc;
    PausewinFunc * lateDispFunc;
    PausewinFunc * deleteFunc;
};
static_assert(sizeof(PausewinEntry) == 0x78);

struct PausewinWork
{
    u32 num;
    PausewinEntry * entries;
    spm::filemgr::FileEntry * pauseTpl;
    s32 entriesUsingPauseTpl;
    spm::filemgr::FileEntry * pause2Tpl;
    s32 entriesUsingPause2Tpl;
    s32 * entryIdQueue;
    s32 curMenuId;
    spm::evtmgr::EvtEntry * itemUseEvt;
    s32 itemUseEvtId;
    u8 unknown_0x28[0x2c - 0x28];
    s32 useItemId;
    s32 subMenuOpenButtonIdx;
    s32 currentPixl;
    bool charPixlItemWinOpen;
    u8 unknown_0x39[0x3c - 0x39];
    f32 gameSpeedBackup;
};
static_assert(sizeof(PausewinWork) == 0x40);

struct PluswinWork
{
    s16 cursorOff;
    u8 unknown_0x2[0x4 - 0x2];
    s32 entryIds[11];
    u8 unknown_0x30[0x84 - 0x30];
    s32 state;
    s32 selectedButton;
    s32 prevSelectedButton;
    wii::Vec2 cursorMoveDest;
    wii::Vec2 cursorPos;
    s8 submenuEntryIds[10];
    u8 unknown_0xaa[0x164 - 0xaa];
    s32 submenuSelectedButton;
    s32 chapterLevel;
    u8 unknown_0x16c[0x184 - 0x16c];
    spm::evtmgr::EvtEntry * keyItemEvt;
    s32 keyItemEvtId;
    u8 unknown_0x18c[0x1a4 - 0x18c];
    s32 selectedItemId;
    u32 submenuFlags;
    bool submenuIsFullClose;
    u8 unknown_0x1ad[0x1b0 - 0x1ad];
    s64 openingTime;
};
static_assert(sizeof(PluswinWork) == 0x1b8);

extern "C" {

extern PluswinWork * pluswinWp;
extern PausewinWork * pausewinWp;

s32 pausewinEntry (f32 x, f32 y, f32 width, f32 height, int boxOnly,
                   PauseTextureId textureId, int priority, PausewinFunc * initFunc,
                   PausewinFunc * mainFunc, PausewinFunc * dispFunc,
                   PausewinFunc * lateDispFunc, PausewinFunc * deleteFunc);
void pausewinMoveTo(s32, f32 x, f32 y);
void pausewinAppear(s32 id);
void pausewinDisappear(s32 id);
void pausewinDelete(s32 id);
void pausewinPauseGame();
void pausewinUnpauseGame();
void pausewinSetMessage(PausewinEntry * entry, s32 itemId, const char * msgName);
void pluswinChapterWinOpen();

}

}
