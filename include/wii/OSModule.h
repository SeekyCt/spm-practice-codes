#pragma once

#include <types.h>

namespace wii::OSModule {

struct RelHeader
{
    u32 id;
    RelHeader * next;
    RelHeader * prev;
    u32 numSections;
    void * sectionInfo;
    void * name;
    u32 nameSize;
    u32 version;
    u32 bssSize;
    void * relocationTable;
    void * impTable;
    u32 impSize;
    u8 prologSection;
    u8 epilogSection;
    u8 unresolvedSection;
    u8 bssSection;
    void * prolog;
    void * epilog;
    void * unresolved;
    u32 align;
    u32 bssAlign;
    u32 fixSize;
};
static_assert(sizeof(RelHeader) == 0x4c);

extern "C" {

extern RelHeader * firstRel;
extern RelHeader * lastRel;

}

}
