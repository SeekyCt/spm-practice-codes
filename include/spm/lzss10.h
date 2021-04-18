#pragma once

#include <types.h>

namespace spm::lzss10 {

/*
struct Lzss10ParsedHeader
{
    u32 flags;
    size_t decompSize;
}
*/

extern "C" {

size_t lzss10GetDecompSize(void * data); // 802e2598
void lzss10Decompress(void * src, void * dest); // 802e25d8
// Lzss10ParsedHeader lzss10ParseHeader(void * data);

}

}
