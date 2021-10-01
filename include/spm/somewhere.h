#pragma once

#include <types.h>

// For functions in unknown files

namespace spm::somewhere {

extern "C" {

// referenced in dan.c
void func_800b426c(float, float, float, int, int);
void func_800cd554(float, float, float, float, float, float, int, int);

}

}
