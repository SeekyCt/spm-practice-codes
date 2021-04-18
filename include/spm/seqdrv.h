#pragma once

#include <spm/seqdef.h>

#include <types.h>

namespace spm::seqdrv {

enum
{
    SEQ_LOGO = 0,
    SEQ_TITLE,
    SEQ_GAME,
    SEQ_MAPCHANGE,
    SEQ_GAMEOVER,
    SEQ_LOAD
};

struct SeqWork
{
    s32 seq; // enum above, or -1 if none
    s32 stage;
    char * p0;
    char * p1;
    u8 unknown_0x10[0x20 - 0x10];
    spm::seqdef::SeqFunc *afterFunc;
};

static_assert(sizeof(SeqWork) == 0x24);

extern "C" {

extern SeqWork seqWork;

void seqInit_SPMARIO();
void seqMain();
void seqSetSeq(s32 seq, const char * p0, const char * p1);
s32 seqGetSeq();

}

}
