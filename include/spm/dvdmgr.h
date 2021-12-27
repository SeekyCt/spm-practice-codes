#pragma once

#include <types.h>

namespace spm::dvdmgr {

struct DVDEntry
{
    u8 unknown_0x0[0x9c - 0x0];
};
static_assert(sizeof(DVDEntry) == 0x9c);

extern "C" {

DVDEntry * DVDMgrOpen(const char * path, u8 priority, s32 param_3); // 8019e08c
s32 DVDMgrRead(DVDEntry * entry, void * dest, size_t length, u32 offset); // 8019e1e4
void DVDMgrClose(DVDEntry * entry); // 8019e2e0
size_t DVDMgrGetLength(DVDEntry * entry); // 8019e320

}

}
