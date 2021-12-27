#pragma once

#include <types.h>
#include <spm/camdrv.h>
#include <wii/tpl.h>
#include <wii/types.h>

namespace spm::gxsub {

extern "C" {

void gxsubInit_Tpl(wii::tpl::TPLHeader * tpl);
void gxsubDrawTextureMtx(u32 texId, wii::Mtx34 * mtx, const wii::RGBA * colour);
void gxsubDrawTexture(u32 texId, f32 x, f32 y, f32 width, f32 height, const wii::RGBA * colour);
float gxsubGetTexWidth(u32 texId);
float gxsubGetTexHeight(u32 texId);
void gxsubEnableTexAlpha();

void gxsubInit(); // general use
void gxsubInit_Cam(spm::camdrv::CamEntry * cam); // gxsubInit + specify active camera
void gxsubInit_Colour(); // sets vertex format to include colours (incompatible with normal functions)
void gxsubDrawQuad();
void gxsubDrawQuadMtx();
void gxsubDrawQuadGradient(); // requires gxsubInit_Colour
void gxsubDrawLineSquare();
void gxsubDrawLineSquareMtx();
void gxsubDrawDottedLine(); // requires no init
void gxsubDrawLine();
void gxsubDrawSphere();

}

}
