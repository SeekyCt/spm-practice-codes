#include <common.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/mem.h>

#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

extern "C" {

void * _malloc_r(struct _reent * reent, size_t size)
{
    return _memalign_r(reent, 0x20, size);
}

void _free_r(struct _reent * reent, void * ptr)
{
    (void) reent;

    if (ptr != nullptr)
        spm::memory::__dl__FPv(ptr);
}

void * _realloc_r(struct _reent * reent, void * ptr, size_t size)
{
    (void) reent;

    // If it fits in the existing memory, just keep using that
    size_t curSize = _malloc_usable_size_r(reent, ptr);
    if (size < curSize)
        return ptr;

    void * newPtr = _malloc_r(reent, size); // sets errno

    if (ptr != nullptr && newPtr != nullptr)
    {
        size_t copySize = curSize > size ? size : curSize; // minimum
        memcpy(newPtr, ptr, copySize);
    }

    _free_r(reent, ptr);

    return newPtr;
}

void * _memalign_r(struct _reent * reent, size_t align, size_t size)
{
    (void) reent;

    wii::mem::MEMHeapHandle handle = spm::memory::memory_wp->heapHandle[spm::memory::HEAP_MAIN];
    void * ret = wii::mem::MEMAllocFromExpHeapEx(handle, size, align);

    if (ret == nullptr)
        errno = ENOMEM;

    return ret;
}

// Broadway 4kb page size
#define PAGE_SIZE 4 * 1024

void * _valloc_r(struct _reent * reent, size_t size)
{
    return _memalign_r(reent, PAGE_SIZE, size);
}

void * _pvalloc_r(struct _reent * reent, size_t size)
{
    void * ret;
    if (size <= SIZE_MAX - PAGE_SIZE)
    {
        size_t roundUp = (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
        ret = _memalign_r(reent, PAGE_SIZE, roundUp);
    }
    else
    {
        ret = nullptr;
    }

    if (ret == nullptr)
        errno = ENOMEM;

    return ret;
}

void * _calloc_r(struct _reent * reent, size_t size, size_t count)
{
    (void) reent;

    // Ideally there would be an overflow check here
    size_t totalSize = size * count;

    void * ret = _malloc_r(reent, totalSize); // sets errno

    if (ret != nullptr)
        memset(ret, 0, totalSize);

    return ret;
}

struct mallinfo _mallinfo_r(struct _reent * reent)
{
    (void) reent;

    assert(false, "mallinfo extension is unuspported");
    return {};
}

int _mallopt_r(struct _reent * reent, int parameter, int value)
{
    (void) reent;
    (void) parameter;
    (void) value;

    // No parameters are supported
    return 0;
}

void _malloc_stats_r (struct _reent * reent)
{
    (void) reent;

    assert(false, "malloc_stats extension is unuspported");
}

int _malloc_trim_r (struct _reent * reent, size_t size)
{
    (void) reent;
    (void) size;

    // Trim has no meaning in context of the game
    return 1;
}

size_t _malloc_usable_size_r (struct _reent * reent, void * ptr)
{
    (void) reent;

    return wii::mem::MEMGetSizeForMBlockExpHeap(ptr);
}

}
