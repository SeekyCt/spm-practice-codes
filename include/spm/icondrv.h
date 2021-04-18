#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::icondrv {

#define ICON_MAX 470

extern "C" {

/*
    Flags:
        16: seems to be required to display
         8: flip y
         4: flip x
        Others unknown
*/
void iconDispGx(float scale, wii::Vec3 * position, u32 flags, s16 type);

}

}
