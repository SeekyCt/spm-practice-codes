#pragma once

#include <types.h>

namespace wii::IPC {

extern "C" {

s32 IOS_Open(const char * path, s32 mode);

}

}
