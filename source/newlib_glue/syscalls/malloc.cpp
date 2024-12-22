/*
    We override Newlib's memory allocation code at a higher level
    Newlib and libsysbase syscalls are stubbed in case any overrides were missed 
*/

#include <common.h>
#include <spm/system.h>

#include <stdlib.h>

extern "C" {

void * _sbrk_r (struct _reent *ptr, ptrdiff_t incr)
{
    (void) ptr;
    (void) incr;

    assert(false, "Newlib tried to use _sbrk syscall");
}

void __syscall_malloc_lock(struct _reent *ptr)
{
    (void) ptr;

    assert(false, "libsysbase tried to use __syscall_malloc_lock");
}

void __syscall_malloc_unlock(struct _reent *ptr)
{
    (void) ptr;

    assert(false, "libsysbase tried to use __syscall_malloc_unlock");
}

}
