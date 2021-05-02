#pragma once

#include <types.h>

namespace spm::spmario_snd {

extern "C" {

void spsndExit();
bool spsndBGMOn(u32 flags, const char * name);

}

}