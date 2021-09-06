#pragma once

#include <types.h>

namespace wii::OSModule {

typedef void (RelFunc)();

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
    RelFunc * prolog;
    RelFunc * epilog;
    RelFunc * unresolved;
    u32 align;
    u32 bssAlign;
    u32 fixSize;
};
static_assert(sizeof(RelHeader) == 0x4c);

extern "C" {

extern RelHeader * firstRel;
extern RelHeader * lastRel;

bool OSLink(RelHeader * rel, void * bss);

}

}
