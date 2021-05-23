#pragma once

#include <wii/types.h>

namespace wii::OSFont {

extern "C" {

int OSGetFontEncode();
void OSLoadFont(void * fontHeader, void * temp);
char * OSGetFontTexel(const char * str, void * image, int pos, int stride, int * width);

}

}
