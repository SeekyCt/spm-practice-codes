#pragma once

#include <common.h>
#include <evt_cmd.h>

namespace mod {

EVT_DECLARE(evt_rel_loader_twice);
extern int relLoaderTwiceEvtId;
void loadMainPatch();

}
