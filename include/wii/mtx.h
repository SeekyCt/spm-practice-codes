#pragma once

#include <wii/types.h>

namespace wii::mtx {

extern "C" {

void PSMTXIdentity(wii::Mtx34 * dest);
void C_MTXOrtho(Mtx44 * dest, float top, float bottom, float left, float right, float near, float far);

}

}
