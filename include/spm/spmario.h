#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::spmario {

struct SpmarioGlobals
{
    u32 flags;
    s32 fps;
    s32 language;
    int discIsEjected;
    u32 systemLevelFlags;
    u8 unknown_0x14[0x18 - 0x14];
    u16 framebufferWidth;
    u16 framebufferHeight;
    f32 gameSpeed;
    char saveName[32];
    int lastSaveLoaded;
    char mapName[32];
    char doorName[32];
    char gameOverMapName[32];
    char prevMapName[32];
    u8 unknown_0xc4[0xc8 - 0xc4];
    s64 lastSaveUpdateTime; // last time save file in memory was written to
    wii::Vec3 savePosition;
    int saveFileId;
    u32 unknown_0xe0;
    u8 unknown_0xe4[0xf8 - 0xe4];
    s64 time;
    u8 unknown_0x100[0x110 - 0x100];
    s64 lastSaveLoadTime;
    u8 unknown_0x118[0x138 - 0x118];
    s64 lastButtonPressTime;
    s32 gsw0;
    u32 gswf[256]; // 8192 flags
    s8 gsw[2048];
    u32 lswf[16]; // 512 flags
    s8 lsw[1024];
    u8 unknown_0x1184[0x900]; // coin related?
    u8 unknown_0x1a84[0x1b08 - 0x1a84];
};
static_assert(sizeof(SpmarioGlobals) == 0x1b08);

extern "C" {

extern SpmarioGlobals * gp;
extern bool doShutdown;

void spmarioInit();
void spmarioMain();
void spmarioDisp();
void spmarioSystemLevel(s32 level);
s32 spmarioGetSystemLevel();
// viPostCallback
// wiiDvdCheckThread
// wiiRumbleCheck
// restartGame
// shutdownConsole
// restartGame2
// returnToWiiMenu

}

}
