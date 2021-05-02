#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::romfont {

struct RomfontEntry
{
    u8 image[0x120];
    s16 character;
    s16 width;
    u8 unknown_0x124[0x140 - 0x124];
};
static_assert(sizeof(RomfontEntry) == 0x140);

struct RomfontWork
{
    RomfontEntry * entries;
    int entryCount;
    int useSJIS;
};
static_assert(sizeof(RomfontWork) == 0xc);

extern "C" {

extern RomfontWork * romfontWp;

void romFontPrintGX(f32 x, f32 y, f32 scale, const wii::RGBA &colour, const char * msg, ...);
int romFontGetWidth(const char * message);

}

}
