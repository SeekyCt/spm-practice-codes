#pragma once

#include <types.h>

namespace spm::evtmgr {

#define EVT_max 0x78
#define EVT_ENTRY_MAX 0x80
#define MAX_EVT_JMPTBL 16

struct EvtEntry;
typedef s32 (user_func)(struct EvtEntry * entry, bool firstRun);
typedef s32 EvtScriptCode;

#define EVT_FLAG_IN_USE (1 << 0) 
#define EVT_FLAG_PAUSED (1 << 1)

struct EvtEntry
{
    s64 lifetime;
    u8 flags; // bit flags, see defines above
    u8 curDataLength; // number of pieces of data (4 bytes each) belonging to the current instruction
    u8 curOpcode;
    u8 priority;
    u8 type;
    s8 blocked;
    s8 dowhileDepth; // number of do-while loops the current instruction is nested into
    s8 unknown_0xf;
    EvtScriptCode * pCurInstruction;
    EvtScriptCode * pCurData;
    s8 labelIds[16]; // each correspond to an address in the jump table
    void * jumptable[16]; // addresses for each label
    struct _EvtEntry * parent;
    struct _EvtEntry * childEntry;
    struct _EvtEntry * brotherEntry;
    s32 unknown_0x74;
    s64 unknown_0x78;
    u8 unknown_0x80[0x94 - 0x80];
    user_func * userFunc; // Function set & called by the user_func opcode
    s32 lw[16];
    u32 lf[3];
    void * dowhileStartPtrs[8]; // pointer to do opcodes
    s32 dowhileCounters[8];
    u8 unknown_0x124[0x150 - 0x124];
    s32 * uw;
    u32 * uf;
    s32 id;
    f32 speed;
    u8 unknown_0x160[0x198 - 0x160];
    EvtScriptCode * scriptStart;
    char * name; // debug thing, unused?
    void * pPrevInstruction;
    u8 unknown_0x1a4[0x1a8 - 0x1a4];
};
static_assert(sizeof(EvtEntry) == 0x1a8);

struct EvtWork
{
    s32 entryCount;
    s32 gw[32];
    u32 gf[3];
    EvtEntry * entries;
    u8 unknown_0x94[0x98 - 0x94];
    s64 time;
};
static_assert(sizeof(EvtWork) == 0xa0);

extern "C" {

EvtWork * evtGetWork();
EvtEntry * evtEntry(const EvtScriptCode * script, u8 priority, u8 flags);
EvtEntry * evtEntryType(const EvtScriptCode * script, u8 priority, u8 flags, s32 type);
EvtEntry * evtChildEntry(EvtEntry * entry, const EvtScriptCode * script, u8 flags);
EvtEntry * evtBrotherEntry(EvtEntry * entry, const EvtScriptCode * script, u8 flags);
bool evtCheckId(s32 id);

// more

}

#define EVT_GET_GW(id) \
    (spm::evtmgr::evtGetWork()->gw[(id)])

#define EVT_SET_GW(id, val) \
    spm::evtmgr::evtGetWork()->gw[(id)] = (val)

#define EVT_GET_GF(id) \
    ((spm::evtmgr::evtGetWork()->gf[(id) / 32] >> ((id) % 32)) & 1)

#define EVT_SET_GF(id) \
    spm::evtmgr::evtGetWork()->gf[(id) / 32] |= 1 << ((id) % 32)

#define EVT_CLEAR_GF(id) \
    spm::evtmgr::evtGetWork()->gf[(id) / 32] &= ~(1 << ((id) % 32))

#define EVT_DECLARE_USER_FUNC(name, parameter_count) \
    constexpr int name##_parameter_count = (parameter_count); \
    s32 name(spm::evtmgr::EvtEntry * evt, bool isFirstCall);

#define EVT_DEFINE_USER_FUNC(name) \
    s32 name(spm::evtmgr::EvtEntry * evt, bool isFirstCall)

#define EVT_DECLARE(name) \
    extern const spm::evtmgr::EvtScriptCode name[];

}
