#include <common.h>
#include <spm/parse.h>
#include <msl/stdio.h>
#include <msl/string.h>

#include "parsepatches.h"
#include "patch.h"

namespace mod {

static void readInt(const char * str, const char * fmt, int * ret)
{
    // If marked as hex, override format to %x
    if (msl::string::strncmp(str, "0x", 2) == 0)
        msl::stdio::sscanf(str + 2, "%x", ret);
    else
        msl::stdio::sscanf(str, fmt, ret); 
}

void parsePatch()
{
    // Replace the int sscanf in parseGet1Next to add hex support
    writeBranchLink(spm::parse::parseGet1Next, 0x2e4, readInt);

    // Change '/n' to '\n' to fix comments
    writeWord(spm::parse::parseGet1Next, 0xb0, 0x2c00000a); // cmpwi r0, '\n'
}

}
