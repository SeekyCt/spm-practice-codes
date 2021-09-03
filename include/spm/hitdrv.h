#pragma once

#include <types.h>

namespace spm::hitdrv {

struct HitObj
{
    u8 unknown_0x0[0x114 - 0x0];
};
static_assert(sizeof(HitObj) == 0x114);

extern "C" {

HitObj * hitNameToPtr(const char * name);

}

}