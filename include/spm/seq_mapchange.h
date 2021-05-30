#pragma once

#include <types.h>

namespace spm::seq_mapchange {

extern "C" {

void _unload(const char * curMapName, const char * nextMapName, const char * nextDoorName);

}

}