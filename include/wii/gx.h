#pragma once

#include <wii/types.h>

namespace wii::GX {

extern "C" {

void GXSetProjection(wii::Mtx44 * mtx, int type);

}

}
