#pragma once

#include <types.h>

namespace wii::MEM {

struct MEMAllocation
{
    u16 magic; // 'UD' / 'FR'
    u16 flags;
    u32 size;
    MEMAllocation * prev;
    MEMAllocation * next;
};
static_assert(sizeof(MEMAllocation) == 0x10);

struct MEMEXPHeap
{
    u32 magic; // 'EXPH'
    MEMEXPHeap * prevHeap;
    MEMEXPHeap * nextHeap;
    u8 unknown_0xc[0x18 -  0xc];
    void * start;
    void * end;
    u8 unknown_0x20[0x3c - 0x20];
    MEMAllocation * firstFree;
    MEMAllocation * lastFree;
    MEMAllocation * firstUsed;
    MEMAllocation * lastUsed;
    u8 unknown_0x4c[0x50 - 0x4c];
};
static_assert(sizeof(MEMEXPHeap) == 0x50);

extern "C" {

u32 MEMGetAllocatableSizeForExpHeapEx(MEMEXPHeap * heap, u32 alignment);
u32 MEMGetSizeForMBlockExpHeap(void * allocation);

}

}

