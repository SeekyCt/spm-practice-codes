#pragma once

#include <types.h>

namespace wii::tpl {

// http://wiki.tockdom.com/wiki/TPL_(File_Format)

// Unions are offsets in files and pointers after loading

enum ImageFormat
{
    IMG_FMT_I4 = 0,
    IMG_FMT_I8 = 1,
    IMG_FMT_IA4 = 2,
    IMG_FMT_IA8 = 3,
    IMG_FMT_RGB565 = 4,
    IMG_FMT_RGB5A3 = 5,
    IMG_FMT_RGBA32 = 6,
    // 7
    IMG_FMT_C4 = 8,
    IMG_FMT_C8 = 9,
    IMG_FMT_C14X2 = 10,
    // 11-13
    IMG_FMT_CMPR = 14
};

#define RGB5A3_SIZE(width, height) ((width) * (height) * 2)
#define CMPR_SIZE(width, height) (((width) * (height)) / 2)

struct ImageHeader
{
    u16 height;
    u16 width;
    u32 format;
    union
    {
        u32 dataOffset;
        void * data;
    };
    u32 wrapS;
    u32 wrapT;
    u32 minFilter;
    u32 magFilter;
    float LODBias;
    bool edgeLODEnable;
    u8 minLOD;
    u8 maxLOD;
    bool unpacked;
};

struct PaletteHeader
{
    u16 entryCount;
    bool unpacked;
    u32 format;
    union
    {
        u32 dataOffset;
        void * data;
    };
};

struct ImageTableEntry
{
    union
    {
        u32 imageOffset;
        ImageHeader * image;
    };
    union
    {
        u32 paletteOffset;
        PaletteHeader * palette;
    };
};

struct TPLHeader
{
    u32 version; // 0x20af30
    u32 imageCount;
    union
    {
        u32 imageTableOffset;
        ImageTableEntry * imageTable;
    };
};

extern "C" {

void TPLBind(TPLHeader * tpl);

}

}
