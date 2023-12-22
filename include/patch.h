#pragma once

#include <common.h>

namespace mod::patch {

void clear_DC_IC_Cache(void * ptr, u32 size);
void _writeBranch(void * ptr, void * destination, bool link = false);
void _writeWord(void * ptr, u32 value);

#define writeBranch(ptr, offset, destination) \
    mod::patch::_writeBranch((void *)((u32)(ptr) + offset), (void *)(destination))
#define writeBranchLink(ptr, offset, destination) \
    mod::patch::_writeBranch((void *)((u32)(ptr) + offset), (void *)(destination), true)
#define writeWord(ptr, offset, value) \
    mod::patch::_writeWord((void *)((u32)(ptr) + (offset)), (value))

// Common instructions
#define NOP 0x60000000
#define BLR 0x4e800020

template<typename Func, typename Dest>
Func hookFunction(Func function, Dest destination)
{
    u32 * instructions = reinterpret_cast<u32 *>(function);
    
    u32 * trampoline = new u32[2];

    // Original instruction
    trampoline[0] = instructions[0];
    clear_DC_IC_Cache(&trampoline[0], sizeof(u32));

    // Branch to original function past hook
    _writeBranch(&trampoline[1], &instructions[1]);
    
    // Write actual hook
    _writeBranch(&instructions[0], reinterpret_cast<void *>(static_cast<Func>(destination)));
    
    return reinterpret_cast<Func>(trampoline);
}

}
