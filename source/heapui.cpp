#include <common.h>
#include <spm/mario.h>
#include <spm/seqdrv.h>
#include <msl/stdio.h>
#include <msl/string.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/mem.h>

#include "mod_ui_base/menuwindow.h"

namespace mod {

#ifdef SPM_EU0
    #define VER "EU0"
#elif defined SPM_EU1
    #define VER "EU1"
#elif defined SPM_US0
    #define VER "US0"
#elif defined SPM_US1
    #define VER "US1"
#elif defined SPM_US2
    #define VER "US2"
#elif defined SPM_JP0
    #define VER "JP0"
#elif defined SPM_JP1
    #define VER "JP1"
#elif defined SPM_KR0
    #define VER "KR0"
#endif

static u32 getHeapSize(wii::mem::MEMEXPHeap * heap)
{
    return (u32) heap->end - (u32) heap->start;
}
static u32 getHeapFreeSpace(wii::mem::MEMEXPHeap * heap)
{
    u32 ret = 0;
    for (wii::mem::MEMAllocation * p = heap->firstFree; p; p = p->next)
        ret += p->size;
    if (getHeapSize(heap) == ret + sizeof(wii::mem::MEMAllocation))
        ret += sizeof(wii::mem::MEMAllocation);
    
    return ret;
}
static u32 getSmartHeapFreeSpace()
{
    using spm::memory::SmartAllocation;
    using spm::memory::memory_swp;
    u32 ret = memory_swp->heapStartSpace;
    for (SmartAllocation * p2 = memory_swp->allocatedStart; p2; p2 = p2->next)
        ret += p2->spaceAfter;
    return ret;
}

static float maxes[HEAP_COUNT];

#define POS_X -365.0f
#define POS_Y 160.0f
#ifdef SPM_KR0
    #define SCALE 1.0f
#else
    #define SCALE 0.6f
#endif

const wii::gx::GXColor pink {247, 114, 255, 255};
const wii::gx::GXColor violet {189, 119, 255, 255};

static void nameSize(char * out, u32 size)
{
    f32 kb = size / 1024.f;
    if (kb < 1024)
    {
        msl::stdio::sprintf(out, "%6.2f KB", kb);
        return;
    }

    f32 mb = kb / 1024.f;
    msl::stdio::sprintf(out, "%6.2f MB", mb);
}

static void sizeInfo(char * out, int i)
{
    (void) i;
    msl::string::strcpy(out, "");

    const auto& entry = spm::memory::memory_size_table[i];
    if (entry.type == spm::memory::HEAPSIZE_ABSOLUTE_KB)
        return;

    msl::stdio::sprintf(out, "%d%%", entry.size);
}

void heapInfo()
{
    wii::os::OSReport("%s\n", VER);
    for (int i = 0; i < HEAP_COUNT; i++)
    {
        const auto& x = spm::memory::memory_size_table[i];
        if (x.type == spm::memory::HEAPSIZE_PERCENT_REMAINING)
            wii::os::OSReport("%d: %d%% of remaining space\n", i, x.size);
        else
            wii::os::OSReport("%d: %dKB\n", i, x.size);
    }
    for (int i = 0; i < HEAP_COUNT; i++)
    {
        assertf(spm::memory::memory_wp->heapHandle[i] == spm::memory::memory_wp->heapStart[i], "Oh no %d", i);
        void * start = spm::memory::memory_wp->heapStart[i];
        void * end = spm::memory::memory_wp->heapEnd[i];
        u32 size = getHeapSize((wii::mem::MEMEXPHeap *) spm::memory::memory_wp->heapHandle[i]);
        char sizeName[64];
        char otherInfo[64];
        nameSize(sizeName, size);
        sizeInfo(otherInfo, i);
        wii::os::OSReport("%d: %p %p | %8d bytes | %s %s\n", i, start, end, size, sizeName, otherInfo);
    }    
}

void heapui()
{
    const wii::gx::GXColor& col = pink;

    Window::drawString(VER, POS_X, POS_Y, &col, SCALE, true);
    for (int i = 0; i < HEAP_COUNT; i++)
    {
        auto p = (wii::mem::MEMEXPHeap *) spm::memory::memory_wp->heapHandle[i];

        u32 free;
        if (i == (s32) spm::memory::HEAP_SMART)
            free = getSmartHeapFreeSpace();
        else
            free = getHeapFreeSpace(p);

        u32 size = getHeapSize(p);

        u32 used = size - free;

        float percent = 100.0f * (float) used / (float) size;

        f32 max = maxes[i];
        if (percent > max)
            max = percent;
        maxes[i] = max;

        char msg[256];
        msl::stdio::sprintf(msg, "%d: %5.2f [%5.2f]\n", i, percent, max);

        Window::drawString(msg, POS_X, POS_Y - ((FONT_HEIGHT * SCALE) * (i+1)), &col, SCALE, true);
    }
}

}
