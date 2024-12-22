#include <spm/system.h>

extern "C" {

void __syscall_abort()
{
    assert(false, "abort() called");
}

}
