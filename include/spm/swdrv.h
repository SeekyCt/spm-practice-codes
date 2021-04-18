#pragma once

#include <types.h>

namespace spm::swdrv {

extern "C" {

void swInit();
void swReInit();
void swSet(s32 id);
bool swGet(s32 id);
void swClear(s32 id);
void swByteSet(s32 id, s32 num);
s32 swByteGet(s32 id);
void _swSet(s32 id);
bool _swGet(s32 id);
void _swClear(s32 id);
void _swByteSet(s32 id, s8 num);
s32 _swByteGet(s32 id);

// more

}

}
