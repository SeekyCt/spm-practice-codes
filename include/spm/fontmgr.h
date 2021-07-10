#pragma once

#include <types.h>

#include <wii/types.h>

namespace spm::fontmgr {

extern "C" {

void FontDrawStart();
void FontDrawStart_alpha(u8 alpha);
void FontDrawEdge();
void FontDrawEdgeOff();
void FontDrawRainbowColor();
void FontDrawRainbowColorOff();
void FontDrawNoise();
void FontDrawNoiseOff();
void FontDrawColorIDX(u32 idx);
// Overwrites color.a
void FontDrawColor(wii::RGBA * color);
void FontDrawColor_();
u32 FontGetDrawColor();
void FontDrawScale(f32 scale);
void FontDrawCode(f32 x, f32 y, u16 code);
void FontDrawString(f32 x, f32 y, const char * string);
void FontDrawMessage(int x, int y, const char * message);
void FontDrawMessageMtx(wii::Mtx34 matrix, const char * message);
u16 FontGetMessageWidth(const char * message);

// more

}

}
