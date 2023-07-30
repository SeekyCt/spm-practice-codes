#pragma once

#include <common.h>
#include <evt_cmd.h>
namespace mod {

#define MOD_VERSION "SPM Practice Codes v9"

extern bool gIsDolphin;
extern bool gIsRiivolution;
extern bool gIsPatchedDisc;
extern bool gIs4_3;
EVT_DECLARE(evt_rel_loader_twice);
extern int relLoaderTwiceEvtId;

void main();

}
