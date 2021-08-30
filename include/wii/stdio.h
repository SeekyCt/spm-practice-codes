#pragma once

namespace wii::stdio {

extern "C" {

int snprintf(char * dest, size_t n, const char * format, ...);
int sprintf(char * dest, const char * format, ...);
int sscanf(const char * str, const char * format, ...);

}

}
