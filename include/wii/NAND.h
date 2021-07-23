#pragma once

#include <types.h>

namespace wii::NAND {

#define NAND_CODE_OK 0
#define NAND_CODE_BUSY -3
#define NAND_CODE_EXISTS -6
#define NAND_CODE_NOT_EXIST -12

#define NAND_PERM_READ 0x10
#define NAND_PERM_WRITE 0x20
#define NAND_PERM_READ_WRITE (NAND_PERM_READ | NAND_PERM_WRITE)

#define NAND_MODE_READ 1
#define NAND_MODE_WRITE 2

struct NANDFileInfo
{
    u8 unknown_0x0[0x8c - 0x0];
};

struct NANDBanner
{
    u32 magic;
    u32 flags;
    u16 iconSpeed;
    u8 unknown_0xa[0x20 - 0xa];
    wchar_t comment[2][32];
    u8 bannerTexture[0x6000];
    u8 iconTextures[8][0x1200];
};

struct NANDCommandBlock
{
    u8 unknown_0x0[0xb8 - 0x0];
#ifdef SPM_KR0
    u8 unknown_0xb8[0xbc - 0xb0];
#endif
};

typedef void (NANDCallback)(s32 result, NANDCommandBlock * commandBlock);

extern "C" {

s32 NANDCreateAsync(const char * filename, u8 permissions, u8 attributes, NANDCallback * callback, NANDCommandBlock * commandBlock);
s32 NANDDeleteAsync(const char * filename, NANDCallback * callback, NANDCommandBlock * commandBlock);
s32 NANDReadAsync(NANDFileInfo * fileInfo, void * buf, u32 length, NANDCallback * callback, NANDCommandBlock * commandBlock);
s32 NANDWriteAsync(NANDFileInfo * fileInfo, void * buffer, u32 size, NANDCallback * callback, NANDCommandBlock * commandBlock);
s32 NANDSafeOpenAsync(const char * path, NANDFileInfo * fileInfo, u8 mode, void * buffer, u32 bufferSize, NANDCallback * callback, NANDCommandBlock * commandBlock);
s32 NANDSafeCloseAsync(NANDFileInfo * fileInfo, NANDCallback * callback, NANDCommandBlock * commandBlock);
s32 NANDChangeDirAsync(const char * path, NANDCallback * callback, NANDCommandBlock * commandBlock);
s32 NANDGetHomeDir(char * path);
s32 NANDCheckAsync(u32 blockCount, u32 inodeCount, u32 * answer, NANDCallback * callback, NANDCommandBlock * commandBlock);

// more

}

}
