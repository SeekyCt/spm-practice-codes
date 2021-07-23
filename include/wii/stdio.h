#pragma once

namespace wii::stdio {

extern "C" {

int sprintf(char * dest, const char * format, ...);
int sscanf(const char * str, const char * format, ...);

}

}
