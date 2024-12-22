#include <common.h>
#include <spm/system.h>

extern "C" {

/*
    GCC will automatically modify main to call this
    Normally this is implemented in libgcc + crt code, but the default implementation would corrupt
    the game's state
*/
void __eabi()
{
    assert(false, "main should not be defined in mod rels, use _prolog");
}

}

