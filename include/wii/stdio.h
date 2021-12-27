#pragma once

#include <wii/stdarg.h>

namespace wii::stdio {

extern "C" {

int vsprintf(char * s, const char * format, va_list arg );
int snprintf(char * dest, size_t n, const char * format, ...);
int sprintf(char * dest, const char * format, ...);
int sscanf(const char * str, const char * format, ...);

}

}
