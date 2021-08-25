#pragma once

#include <spm/evtmgr.h>
#include <spm/nandmgr.h>

namespace mod {

#define ARRAY_SIZEOF(ary) (sizeof((ary))/sizeof((ary)[0]))

#define NYBBLE_0(n) (((n) >> 28) & 0xf)
#define NYBBLE_1(n) (((n) >> 24) & 0xf)
#define NYBBLE_2(n) (((n) >> 20) & 0xf)
#define NYBBLE_3(n) (((n) >> 16) & 0xf)
#define NYBBLE_4(n) (((n) >> 12) & 0xf)
#define NYBBLE_5(n) (((n) >> 8)  & 0xf)
#define NYBBLE_6(n) (((n) >> 4)  & 0xf)
#define NYBBLE_7(n) ( (n)        & 0xf)

#define DIGIT_1000(n) ( (n) / 1000)
#define DIGIT_100(n) (((n) % 1000) / 100)
#define DIGIT_10(n) (((n) % 100)  / 10)
#define DIGIT_1(n) ( (n) % 10)

#define IS_BOOL(n) (((n) & ~0x1)  == 0)
#define IS_BYTE(n) (((n) & ~0xff) == 0)

#define ROUND_UP_32(n) (((n) + 0x1f) & ~0x1f)

#define BYTES_TO_KB(size) ((((size) + 1023) & ~1023) / 1024)
#define KB_TO_BLOCKS(size) ((((size) + 15) & ~15) / 16)
#define BYTES_TO_BLOCKS(size) (KB_TO_BLOCKS(BYTES_TO_KB((size))))

// evt_get_cur_pixl(&ret)
EVT_DECLARE_USER_FUNC(evt_get_cur_pixl, 1)

// evt_freeze_game()
EVT_DECLARE_USER_FUNC(evt_freeze_game, 0)

// evt_unfreeze_game()
EVT_DECLARE_USER_FUNC(evt_unfreeze_game, 0)

void updateSaveChecksum(spm::nandmgr::SaveFile * save);

const char * getGameRegion();
int getGameRevision();
void * getModRelLoadAddr();

const char * getToggleName(bool value);

// Doesn't support negative powers
s32 pow(s32 val, s32 power);

bool check3d();

}
