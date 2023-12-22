#pragma once

#include <common.h>

// Forward declaration
CPP_WRAPPER(wii::PAD)

struct _PADOrigin;

CPP_WRAPPER_END()

CPP_WRAPPER(wii::SI)

USING(wii::PAD::_PADOrigin)

typedef struct _SIData
{
    s32 channel;
} SIData; // Unknown total size

typedef struct _SIPacket
{
    s32 unknown_0x0;
    u8 unknown_0x4[0x20 - 0x4];
} SIPacket;
static_assert(sizeof(SIPacket) == 0x20);

typedef struct _SIResponse
{
    u32 unknown_0x0;
    u32 unknown_0x4;
} SIResponse;
static_assert(sizeof(SIResponse) == 0x8);

typedef void (SITransferCB)(s32, u32);
typedef void (SIGetTypeCB)(s32, u32);

extern SIData Si; // 804e9b80
extern SIPacket Packet[4]; // 80537f98
extern u32 Type[4]; // 804e9b98

void SIDisablePolling(u32 mask); // 802b2438
void SITransfer(s32 chan, u32 * cmd, s32 p3, struct _PADOrigin * origin,
                s32 p5, SITransferCB cb, s32 p7, s32 p8);
u32 SIGetType(s32 chan); // 8026d7a0
s32 SIGetResponse(s32 chan, SIResponse * out);
s32 SIGetTypeAsync(s32 chan, SIGetTypeCB cb);


// Re-implemented in game_lib_ext:
bool SIIsChanBusy(s32 chan);
u32 SIGetStatus(s32 chan);

CPP_WRAPPER_END()
