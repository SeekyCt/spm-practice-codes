#pragma once

namespace spm::relmgr {

struct RelWork
{
    void * relFile;
    void * bss;
    bool loaded;
};

static_assert(sizeof(RelWork) == 0xc);

extern "C" {

extern RelWork * relWp;
extern const char * relDecompName; // "relF.rel"
extern const char * relCompName; // "relF.bin"

void relInit();
void loadRel();
bool isRelLoaded();

}

}
