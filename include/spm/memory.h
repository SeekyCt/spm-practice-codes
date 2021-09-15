#pragma once

#include <types.h>
#include <wii/mem.h>

// Would be cyclic include
namespace spm::filemgr { struct FileEntry; }

namespace spm::memory {

#define MEM1_HEAP_COUNT 3
#define MEM2_HEAP_COUNT 6
#define HEAP_COUNT 9
#define SMART_HEAP_ID 7
#define SMART_ALLOCATION_MAX 2048

struct SmartAllocation
{
    void * data; // space on the smart heap for user to put their data
    size_t size; // size of the space for data on the smart heap
    struct spm::filemgr::FileEntry * fileEntry; // allows special treatment if this allocation is for a file
    u16 flag; // 1 for in use, 0 otherwise
    u8 type; // used to group for deallocation
    u8 unknown_0xf; // padding?
    size_t spaceAfter; // amount of free space in the smart heap after the data of this allocation
    SmartAllocation * next; // next item in the allocated or free linked list
    SmartAllocation * prev; // previous item in the allocated or free linked list
};
static_assert(sizeof(SmartAllocation) == 0x1c);

struct SmartWork
{
    void * heapStart;
    SmartAllocation allocations[2048];
    u32 heapStartSpace;
    SmartAllocation * allocatedStart;
    SmartAllocation * allocatedEnd;
    SmartAllocation * freeStart;
    SmartAllocation * freeEnd;
    u32 freedThisFrame;    
};
static_assert(sizeof(SmartWork) == 0xe01c);

struct MemWork
{
    wii::MEM::MEMEXPHeap * heapHandle[HEAP_COUNT];
    void * heapStart[HEAP_COUNT];
    void * heapEnd[HEAP_COUNT];
};
static_assert(sizeof(MemWork) == 0x6c);

enum HeapSizeType
{
    HEAPSIZE_PERCENT_REMAINING,
    HEAPSIZE_ABSOLUTE_KB
};

struct HeapSize
{
    s32 type;
    s32 size;
};
static_assert(sizeof(HeapSize) == 0x8);

enum class Heap
{
    MAIN = 0,
    MAP = 1,
    MEM1_UNUSED = 2,
    EXT = 3,
    EFFECT = 4,
    WPAD = 5,
    SOUND = 6,
    SMART = 7,
    MEM2_UNUSED = 8
};

extern "C" {

extern MemWork * memoryWp;
extern SmartWork * memorySwp;
extern HeapSize size_table[HEAP_COUNT];

void *__memAlloc(Heap heap, u32 size);
void __memFree(Heap heap, void * ptr);
void __dl__FPv(void *);

}

}

// custom in cxx.cpp
void * operator new(std::size_t size, spm::memory::Heap heap);
void * operator new[](std::size_t size, spm::memory::Heap heap);
