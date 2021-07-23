#include "parsepatches.h"
#include "patch.h"

#include <types.h>
#include <spm/parse.h>
#include <wii/stdio.h>
#include <wii/string.h>

namespace mod {

static void readInt(const char * str, const char * fmt, int * ret)
{
    // If marked as hex, override format to %x
    if (wii::string::strncmp(str, "0x", 2) == 0)
        wii::stdio::sscanf(str + 2, "%x", ret);
    else
        wii::stdio::sscanf(str, fmt, ret); 
}

void parsePatch()
{
    // Replace the int sscanf in parseGet1Next to add hex support
    writeBranchLink(spm::parse::parseGet1Next, 0x2e4, readInt);

    // Change '/n' to '\n' to fix comments
    writeWord(spm::parse::parseGet1Next, 0xb0, 0x2c00000a); // cmpwi r0, '\n'
}

}
