#pragma once

#include <common.h>

namespace mod {

/// Checks if there is a USB Gecko on EXI channel `chan`
bool ugProbe(s32 chan);

/// Try to send `len` bytes from `data` via the USB Gecko on channel `chan`.
/// Returns number of bytes sent or negative on error.
s32 ugSend(s32 chan, const void * data, s32 len);

/// Try to receive `len` bytes into `data` via the USB Gecko on channel `chan`.
/// Returns number of bytes received or negative on error.
s32 ugRecv(int chan, void * data, s32 len);

}
