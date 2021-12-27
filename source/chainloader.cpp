#include "chainloader.h"
#include "consolewindow.h"

#include <types.h>
#include <spm/memory.h>
#include <spm/dvdmgr.h>
#include <wii/DVDFS.h>
#include <wii/OSModule.h>

namespace mod {

#define MOD_CHAINLOAD_PATH "./mod/chain.rel"

using spm::memory::Heap;
using spm::dvdmgr::DVDEntry;
using wii::OSModule::RelHeader;

using spm::dvdmgr::DVDMgrOpen;
using spm::dvdmgr::DVDMgrGetLength;
using spm::dvdmgr::DVDMgrRead;
using spm::dvdmgr::DVDMgrClose;
using wii::DVDFS::DVDConvertPathToEntrynum;
using wii::OSModule::OSLink;

void tryChainload()
{
    if (DVDConvertPathToEntrynum(MOD_CHAINLOAD_PATH) == -1)
        return ConsoleWindow::push("No mod to chainload found");
    
    DVDEntry * f = DVDMgrOpen(MOD_CHAINLOAD_PATH, 2, 0);

    size_t len = DVDMgrGetLength(f);
    void * buf = new (Heap::MEM1_UNUSED) u8[len];
    
    DVDMgrRead(f, buf, len, 0);

    DVDMgrClose(f);

    RelHeader * rel = reinterpret_cast<RelHeader *>(buf);

    void * bss = new (Heap::MEM1_UNUSED) u8[rel->bssSize];
    
    OSLink(rel, bss);
    rel->prolog();
}

}
