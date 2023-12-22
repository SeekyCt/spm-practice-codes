#pragma once
#include <wii/pad.h>
namespace mod {

extern wii::PAD::PADStatus pads[4];
extern wii::PAD::PADStatus prevPads[4];

void gcpadPatch();

}

