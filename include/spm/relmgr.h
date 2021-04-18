#pragma once

namespace spm::relmgr {

struct RelHolder
{
    void * relFile;
    void * bss;
    bool loaded;
};

static_assert(sizeof(RelHolder) == 0xc);

extern "C" {

extern RelHolder * relHolder;
extern const char * relDecompName; // "relF.rel"
extern const char * relCompName; // "relF.bin"

void relInit();
void loadRel();
bool isRelLoaded();

}

}
