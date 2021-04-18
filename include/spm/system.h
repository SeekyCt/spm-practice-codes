#pragma once

#include <types.h>

namespace spm::system {

extern "C" {

#define assert(condition, message) \
    if (!(condition)) spm::system::__assert2(__FILE__, __LINE__, #condition, message)

#define assertf(condition, message, ...) \
    if (!(condition)) spm::system::__assert2(__FILE__, __LINE__, #condition, message, __VA_ARGS__)

int __assert2(const char * filename, int line, const char * assertion, const char * message,...);

int irand(int limit);

}

}
