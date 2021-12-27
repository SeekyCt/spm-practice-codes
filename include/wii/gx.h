// Parameter names based off of libogc's gx.h

#pragma once

#include <wii/types.h>

namespace wii::GX {

extern "C" {

void GXSetVtxDesc(u8 attr, u8 type);
void GXClearVtxDesc();
void GXSetVtxAttrFmt(u8 vtxfmt, u32 vtxattr, u32 comptype, u32 compsize, u32 frac);
void GXSetNumTexGens(u32 nr);
void GXBegin(u8 primitve, u8 vtxfmt, u16 vtxcnt);
void GXSetCullMode(u8 mode);
void GXSetNumChans(u8 num);
void GXSetChanCtrl(s32 channel, u8 enable, u8 ambsrc, u8 matsrc, u8 litmask, u8 diff_fn, u8 attn_fn);
void GXSetTevOp(u8 tevstage, u8 mode);
void GXSetAlphaCompare(u8 comp0, u8 ref0, u8 aop, u8 comp1, u8 ref1);
void GXSetTevOrder(u8 tevstage, u8 texcoord, u32 texmap, u8 color);
void GXSetNumTevStages(u8 num);
void GXSetFog(u8 type, f32 startz, f32 endz, f32 nearz, f32 farz, const wii::RGBA * col);
void GXSetBlendMode(u8 type, u8 src_fact, u8 dst_fact, u8 op);
void GXSetZMode(u8 enable, u8 func, u8 update_enable);
void GXSetZCompLoc(u8 before_tex);
void GXSetProjection(wii::Mtx44 * mtx, u32 type);
void GXLoadPosMtxImm(wii::Mtx34 * mtx, u32 pnidx);
void GXSetCurrentMtx(u32 mtx);

}

}
