#pragma once

#include <wii/types.h>

namespace wii::mtx {

extern "C" {

void PSMTXIdentity(wii::Mtx34 * dest);
void C_MTXOrtho(Mtx44 * dest, f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far);

}

}
