#include <spm/system.h>

extern "C" {

void __syscall_exit(int code)
{
    assertf(false, "exit() called with code %d", code);
}

}
