#pragma once

#include <common.h>
#include <wii/OS.h>

namespace clibcompat {

typedef u32 Trampoline[2];

inline void clear_DC_IC_Cache(void * ptr, u32 size)
{
    wii::os::DCFlushRange(ptr, size);
    wii::os::ICInvalidateRange(ptr, size);
}

inline void _writeBranch(void * ptr, void * destination, bool link)
{
    u32 delta = reinterpret_cast<u32>(destination) - reinterpret_cast<u32>(ptr);
    u32 value = link ? 0x48000001 : 0x48000000;
    value |= (delta & 0x03FFFFFC);
    
    u32 * p = reinterpret_cast<u32 *>(ptr);
    *p = value;

    clear_DC_IC_Cache(ptr, sizeof(u32));
}

inline void _writeWord(void * ptr, u32 value)
{
    u32 * p = reinterpret_cast<u32 *>(ptr);
    *p = value;

    clear_DC_IC_Cache(ptr, sizeof(u32));
}

template <typename T1, typename T2>
void writeBranch(T1 * ptr, u32 offset, T2 * destination, bool link=false)
{
    _writeBranch((void *)((u32)ptr + offset), (void *)destination, link);
}

template <typename T1>
void writeWord(T1 * ptr, u32 offset, u32 value)
{
    _writeWord((void *)((u32)(ptr) + (offset)), (value));
}

template<typename Func, typename Dest>
Func hookFunction(Func function, Dest destination, Trampoline trampoline)
{
    u32 * instructions = reinterpret_cast<u32 *>(function);
    
    // Original instruction
    _writeWord(&trampoline[0], instructions[0]);

    // Branch to original function past hook
    _writeBranch(&trampoline[1], &instructions[1], false);
    
    // Write actual hook
    _writeBranch(&instructions[0], reinterpret_cast<void *>(static_cast<Func>(destination)), false);
    
    return reinterpret_cast<Func>(trampoline);
}

}
