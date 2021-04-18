#pragma once

#include <wii/types.h>

namespace wii::OSError {

extern "C" {

void OSReport(const char * message, ...);
void OSFatal(const RGBA * foreground, const RGBA * background, const char * messsage);

}

}
