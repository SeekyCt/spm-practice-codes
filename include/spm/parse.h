#pragma once

#include <types.h>

namespace spm::parse {

enum ParseValueType
{
    PARSE_VALUE_TYPE_STRING = 0,
    PARSE_VALUE_TYPE_1 = 1,
    PARSE_VALUE_TYPE_INT = 2,
    PARSE_VALUE_TYPE_FLOAT = 3,
    PARSE_VALUE_TYPE_VEC3 = 4
};

extern "C" {

void parseInit(const char * text, u32 length);
// getTag
bool parsePush(const char * tag);
void parsePop();
void parsePopNext();
bool parseGet1Next(int type, void * ret);
bool parseTagGet1(const char * tag, int type, void * ret);

}

}
