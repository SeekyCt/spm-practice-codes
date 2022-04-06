#pragma once

#include <types.h>

namespace wii::IPC {

struct Ioctlv
{
    void * data;
    u32 len;
};

extern "C" {

s32 IOS_Open(const char * path, s32 mode);
s32 IOS_Close(s32 fd);
s32 IOS_Ioctlv(s32 fd, s32 command, s32 inCount, s32 outCount, Ioctlv * vecs);

}

}
