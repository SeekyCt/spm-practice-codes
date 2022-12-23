#include <common.h>
#include <wii/os.h>

#include "patch.h"

namespace mod::patch {

void clear_DC_IC_Cache(void * ptr, u32 size)
{
    wii::os::DCFlushRange(ptr, size);
    wii::os::ICInvalidateRange(ptr, size);
}

void _writeBranch(void * ptr, void * destination, bool link)
{
    u32 delta = reinterpret_cast<u32>(destination) - reinterpret_cast<u32>(ptr);
    u32 value = link ? 0x48000001 : 0x48000000;
    value |= (delta & 0x03FFFFFC);
    
    u32 * p = reinterpret_cast<u32 *>(ptr);
    *p = value;

    clear_DC_IC_Cache(ptr, sizeof(u32));
}

void _writeWord(void * ptr, u32 value)
{
    u32 * p = reinterpret_cast<u32 *>(ptr);
    *p = value;

    clear_DC_IC_Cache(ptr, sizeof(u32));
}

}