#include "./devices/stdio.h"
#include "./syscalls/reent.h"

namespace clibcompat {

void init()
{
    stdioPatch();
    reentPatch();
}

}
