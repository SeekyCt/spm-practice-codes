#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::windowdrv {

extern "C" {

void windowDispGX2_Waku_col(const wii::Mtx34 * mtx, u16 GXTexMapID, const wii::RGBA * colour, f32 x, f32 y, f32 width, f32 height, f32 curve);

}

}
