#pragma once

#include <types.h>

namespace wii::string {

extern "C" {

void * memcpy(void * dest, const void * src, u32 n);
void * memset(void * ptr, int value, u32 n);
void * memmove(void * dest, const void * src, u32 n);
char * strcpy(char * dest, const char * src);
char * strncpy(char * dest, const char * src, size_t n);
s32 strcmp(const char * s1, const char * s2);
s32 strncmp(const char * s1, const char * s2, u32 n);
const char * strchr(const char * str, char c);
size_t strlen(const char * str);
const char * strstr(const char * str1, const char * str2);

// more

}

}
