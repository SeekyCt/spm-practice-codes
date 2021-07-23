#pragma once

#include <types.h>
#include <spm/memory.h>

// Would be cyclic include
namespace spm::memory { struct SmartAllocation; }

namespace spm::filemgr {

struct FileEntry
{
    u8 state;
    s8 fileType;
    s16 touchCnt;
    u8 unknown_0x4[0xa4 - 0x4];
    struct spm::memory::SmartAllocation * sp; // smart pointer to file data
    FileEntry * next; // next entry in free or allocated list
    u8 unknown_0xac[0xb0 - 0xac];
    void * dvdEntry;
};
static_assert(sizeof(FileEntry) == 0xb4);

extern "C" {

FileEntry * fileAllocf(int filetype, const char * format, ...);
void fileFree(FileEntry * file);
FileEntry * fileAsyncf(int filetype, void * readDoneCb, const char * format, ...);

}

}
