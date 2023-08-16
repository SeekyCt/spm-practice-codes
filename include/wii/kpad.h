#pragma once

#include <types.h>
#include <wii/types.h>

namespace wii::KPAD {

struct KPADStatus
{
    u32 buttonsHeld; // all buttons down
    u32 buttonsPressed; // buttons down this frame but not last
    u32 buttonsReleased; // buttons released this frame
    wii::Vec3 acceleration;
    float accelerationMagnitude;
    float accelerationDif;
    wii::Vec2 pointingPos;
    wii::Vec2 pointingPosDif;
    float pointingPosDifSpeed;
    wii::Vec2 horizontal;
    wii::Vec2 horizontalDif;
    float horizontalDifSpeed;
    float distance;
    float distanceDif;
    float distanceDifSpeed;
    wii::Vec2 verticalPos;
    u8 type;
    s8 error;
    s8 dpdStatus;
    u8 dataFormat;
    u8 extension[0x24];
};

extern "C" {

s32 KPADRead(s32 chan, KPADStatus * statuses, u32 length);

}

}
