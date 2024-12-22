# C and C++ Standard Library Support

The C standard library should be fully safe to use in mods with this setup, though some parts are unimplemented and will trigger an assert failure when called.

The C++ standard library is supported to the extent that it is in regular DevkitPPC, meaning threading isn't supported and some areas may lack the thread-safety they're supposed to have. Many parts of the library will greatly inflate binary size, however, so be cautious when using it. (For example, one call to std::cout links in hundreds of kilobytes of code).

## Requirements

DevkitPPC release 46.1 minimum is required. Older versions will definitely break, new ones hopefully won't but are currently untested.

## Context

DevkitPPC uses Newlib for its C library. Newlib handles portability by expecting other code to provide syscall functions (such as `_exit). DevkitPPC provides these through libsysbase, which in turn requires a simpler set of syscall functions to be provided from other code (such as __syscall_exit). These are expected to be provided by libogc, however libogc isn't fully safe to use inside of a running game, so replacements for its syscalls are provided here.

Newlib also expects some things (such as `__fini`) to be provided by the compiler's CRT code. We also can't safely use the regular CRT, since it would interfere with the running game, so some replacements are included where needed.

For the C++ library, DevkitPPC uses GCC's libstdc++. libstdc++ handles portability through the gthread interface, which DevkitPPC provides stubs for (gthr-dkp in libgcc). Users are optionally able to provide `__gthr_impl_*` to actually give the functionality, but libogc doesn't do that and neither does this setup, leaving some parts of the library non-functional and others potentially not threadsafe.

## Approach

Where possible, everything is done by implementing syscalls for libsysbase to use.

### Memory Allocation

Newlib and libsysbase assume they can have full control over a contiguous block of memory to do their allocations within. Newlib uses the _sbrk syscall to expand/shrink this block, and libsysbase implements this by getting the end address of the binary from the linker and simply taking the memory after this for itself.

The libsysbase method is obviously incompatible with a rel binary allocated in the middle of a game's heap. While we could implement _sbrk ourselves to just take a chunk of the game's heap and give it to Newlib, this would either waste a lot of memory when it's not fully in use or risk running out despite the game as a whole having lots of memory free. Therefore, we override the functions from Newlib's _mallocr.c with our own that use the game's heaps.

Ideally this would be done by building Newlib with the MALLOC_PROVIDED define, but we want to avoid rebuilding the library, and it's pretty much equivalent to just replacing the symbols anyway. 

## Extensions

Newlib also includes some extensions to the C library (some GNU and UNIX functions). Not all of these will be functional, but they should at least be safe to call without hidden bugs.

## IO devices

stdout and stderr are set up to print to OSReport. Mods are also able to register their own devices through `<sys/iosupport.h>` from DevkitPPC. I will possibly create devices for accessing the NAND and DVD (and maybe SD card?) in the future, but they would be distributed separately.

## Supported libsysbase syscalls

- [x] __syscall_exit
- [x] __syscall_abort
- [x] __syscall_assert_func
- [ ] __syscall_clock_gettime
- [ ] __syscall_clock_settime
- [ ] __syscall_clock_getres
- [x] __syscall_getreent
- [ ] __syscall_gettod_r
- [ ] __syscall_nanosleep
- [x] __syscall_lock_init
- [x] __syscall_lock_acquire
- [x] __syscall_lock_try_acquire
- [x] __syscall_lock_release
- [x] __syscall_lock_close
- [x] __syscall_lock_init_recursive
- [x] __syscall_lock_acquire_recursive
- [x] __syscall_lock_try_acquire_recursive
- [x] __syscall_lock_release_recursive
- [x] __syscall_lock_close_recursive
- [x] __syscall_malloc_lock
- [x] __syscall_malloc_unlock

All unsupported syscalls should safely fail with an assert.

## Supported C headers

| Header        | Support | Note
|---------------|---------|-----
| assert.h      | Yes     |
| complex.h     | Yes     |
| ctype.h       | Yes     |
| errno.h       | Yes     |
| fenv.h        | Yes     |
| float.h       | Yes     |
| inttypes.h    | Yes     |
| iso646.h      | Yes     |
| limits.h      | Yes     |
| locale.h      | Yes     |
| math.h        | Yes     |
| setjmp.h      | Yes     |
| signal.h      | Yes     |
| stdalign.h    | Yes     |
| stdarg.h      | Yes     | 
| stdatomic.h   | Partial | Only hardware-supported atomics work, software would require -latomic which DevkitPPC doesn't include
| stdbool.h     | Yes     |
| stddef.h      | Yes     |
| stdint.h      | Yes     |
| stdio.h       | Yes     |
| stdlib.h      | Yes     |
| stdnoreturn.h | Yes     |
| string.h      | Yes     |
| tgmath.h      | Yes     |
| threads.h     | No      | Not functional in regular DevkitPPC environment either (no `<machine/_threads.h>` exists)
| time.h        | No      | Needs more syscalls
| wchar.h       | Yes     |
| wctype.h      | Yes     |

## Supported C++ headers

This has been less tested, but most parts that aren't related to threading should work.

