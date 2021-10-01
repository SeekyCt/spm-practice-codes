#pragma once

namespace spm::mobjdrv {

struct MOBJEntry
{
    u8 unknown_0x0[0x18 - 0x0];
    wii::Vec3 pos;
    u8 unknown_0x24[0x2a8 - 0x24];
};
static_assert(sizeof(MOBJEntry) == 0x2a8);

extern "C" {

MOBJEntry * mobjNameToPtr(const char * name);
MOBJEntry * mobjNameToPtrNoAssert(const char * name);

}

}
