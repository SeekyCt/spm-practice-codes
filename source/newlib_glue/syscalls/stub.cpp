#include <common.h>
#include <spm/system.h>

#include <stddef.h>

extern "C" {

#define DUMMY_SYSCALL(name) void name() { assert(false, "Unsupported syscall " #name "\n"); }

// DUMMY_SYSCALL(__syscall_exit)
// DUMMY_SYSCALL(__syscall_abort)
// DUMMY_SYSCALL(__syscall_assert_func)
DUMMY_SYSCALL(__syscall_clock_gettime)
DUMMY_SYSCALL(__syscall_clock_settime)
DUMMY_SYSCALL(__syscall_clock_getres)
// DUMMY_SYSCALL(__syscall_getreent)
DUMMY_SYSCALL(__syscall_gettod_r)
DUMMY_SYSCALL(__syscall_nanosleep)
// DUMMY_SYSCALL(__syscall_lock_init)
// DUMMY_SYSCALL(__syscall_lock_acquire)
// DUMMY_SYSCALL(__syscall_lock_try_acquire)
// DUMMY_SYSCALL(__syscall_lock_release)
// DUMMY_SYSCALL(__syscall_lock_close)
// DUMMY_SYSCALL(__syscall_lock_init_recursive)
// DUMMY_SYSCALL(__syscall_lock_acquire_recursive)
// DUMMY_SYSCALL(__syscall_lock_try_acquire_recursive)
// DUMMY_SYSCALL(__syscall_lock_release_recursive)
// DUMMY_SYSCALL(__syscall_lock_close_recursive)
// DUMMY_SYSCALL(__syscall_malloc_lock)
// DUMMY_SYSCALL(__syscall_malloc_unlock)

}
