#pragma once

#include "evt_cmd.h"

namespace spm::hud {

typedef void (CountdownDoneCb)();

extern "C" {

void hudHide();
void hudLoadStats();
void hudStartCountdown(u32 length, CountdownDoneCb * cb);

}

}