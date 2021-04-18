#pragma once

#include <types.h>

namespace wii::string {

extern "C" {

void * memcpy(void * dest, void * src, u32 n);
void * memset(void * ptr, int value, u32 n);
void * memmove(void * dest, const void * src, u32 n);
char * strcpy(char * dest, const char * src);
s32 strcmp(const char * s1, const char * s2);
s32 strncmp(const char * s1, const char * s2, u32 n);
s32 sscanf(const char * s, const char * format, ...);
size_t strlen(const char * str);

// more

}

}
