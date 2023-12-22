#pragma once

#include <common.h>

CPP_WRAPPER(wii::SI)

struct _SIResponse;

CPP_WRAPPER_END()

CPP_WRAPPER(wii::PAD)

USING(wii::SI::_SIResponse)

#define PAD_BTN_LEFT 0x1
#define PAD_BTN_RIGHT 0x2
#define PAD_BTN_DOWN 0x4
#define PAD_BTN_UP 0x8
#define PAD_BTN_Z 0x10
#define PAD_BTN_R 0x20
#define PAD_BTN_L 0x40
#define PAD_BTN_A 0x100
#define PAD_BTN_B 0x200
#define PAD_BTN_X 0x400
#define PAD_BTN_Y 0x800
#define PAD_BTN_START 0x1000

typedef struct _PADStatus
{
    u16 buttonsPressed;
    s8 lStickX;
    s8 lStickY;
    s8 rStickX;
    s8 rStickY;
    u8 lTrigger;
    u8 rTrigger;
    u8 aAnalogue;
    u8 bAnalogue;
    s8 error;
    u8 unknown_0xb[0xc - 0xb];
} PADStatus;
static_assert(sizeof(PADStatus) == 0xc);

typedef struct _PADOrigin
{
    u8 unknown_0x0[0xc - 0x0];
} PADOrigin;
static_assert(sizeof(PADOrigin) == 0xc);

extern PADOrigin Origin[4]; // 805a8f30

extern u32 EnabledBits; // 805af12c
extern u32 WaitingBits; // 805af120
extern u32 CheckingBits; // 805af11c
extern u32 PendingBits; // 80415a78
extern u32 BarrelBits; // 80415a7c
extern u32 ResettingBits; // 805af128

extern s32 ResettingChan; // 805ae664

extern u32 CmdReadOrigin;

extern void (*MakeStatus)(s32 chan, PADStatus * pad, _SIResponse * siResponse); // 805ae674

void UpdateOrigin(s32 chan); // 802e3300
bool PADReset(u32 mask); // 802e3958


// Re-implemented in game_lib_ext:
void PADOriginUpdateCallback(s32 chan, s32 flags);
void PADReceiveCheckCallback(s32 chan, u32 flags);
u32 PADRead(PADStatus * pads);

CPP_WRAPPER_END()
