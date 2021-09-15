#pragma once

#include <types.h>
#include <wii/types.h>

namespace spm::icondrv {

enum
{
    ICON_BTN_WII_REMOTE = 0,
    ICON_BTN_1_UP,
    ICON_BTN_1_DOWN,
    ICON_BTN_2_UP,
    ICON_BTN_2_DOWN,
    ICON_BTN_A_UP,
    ICON_BTN_A_DOWN,

    ICON_DPAD_NEUTRAL = 9,
    ICON_BTN_A,
    ICON_BTN_POWER,
    ICON_BTN_HOME,
    ICON_BTN_MINUS,
    ICON_BTN_PLUS,
    ICON_BTN_1,
    ICON_BTN_2,

    ICON_DPAD_UP_PROMPT = 37,
    ICON_DPAD_DOWN_PROMPT,
    ICON_DPAD_LEFT_PROMPT,
    ICON_DPAD_RIGHT_PROMPT,
    ICON_DPAD_NEUTRAL_PROMPT,
    ICON_DPAD_UP_PROMPT_FLASH,
    ICON_DPAD_DOWN_PROMPT_FLASH,
    ICON_DPAD_LEFT_PROMPT_FLASH,
    ICON_DPAD_RIGHT_PROMPT_FLASH,
    ICON_BTN_1_PRESSING,
    ICON_BTN_2_PRESSING,
    ICON_BTN_A_PRESSING,
    
    // more
};

extern "C" {

/*
    Flags:
        16: seems to be required to display
         8: flip y
         4: flip x
        Others unknown
*/
void iconDispGx(f32 scale, wii::Vec3 * position, u32 flags, s16 type);

}

}
