#pragma once

#include <wii/types.h>

namespace wii::mtx {

extern "C" {

void PSMTXIdentity(wii::Mtx34 * dest);
void PSMTXTrans(wii::Mtx34 * dest, f32 x, f32 y, f32 z);
void PSMTXTransApply(wii::Mtx34 * src, wii::Mtx34 * dest, f32 x, f32 y, f32 z);
void PSMTXScale(wii::Mtx34 * dest, f32 x, f32 y, f32 z);
void PSMTXScaleApply(wii::Mtx34 * src, wii::Mtx34 * dest, f32 x, f32 y, f32 z);
void C_MTXOrtho(wii::Mtx44 * dest, f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far);

}

}
