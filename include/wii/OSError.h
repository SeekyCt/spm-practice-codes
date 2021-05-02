#pragma once

#include <wii/types.h>

namespace wii::OSError {

extern "C" {

extern char exceptionMsg1[];
extern char exceptionMsg2[];
extern char exceptionMsg3[];

void OSReport(const char * message, ...);
void __OSUnhandledException(int p1, int p2, int p3, int p4);
void OSFatal(const RGBA * foreground, const RGBA * background, const char * messsage);

// The game overrides this symbol with its own
void OSPanic(const char * filename, int line, const char * msg, ...);

}

}
