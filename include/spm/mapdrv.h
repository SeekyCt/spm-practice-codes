#pragma once

#include <types.h>

namespace spm::mapdrv {

#define MAPOBJ_FLAG_HIDE 0x1

extern "C" {

void mapGrpFlagOn(bool allowSubname, const char * name, u32 mask); // 8008cf30
void mapGrpFlagOff(bool allowSubname, const char * name, u32 mask); // 8008d2fc
void mapGrpFlag4On(bool allowSubname, const char * name, u32 mask); // 8008e11c

}

}
