/*
    We treat _LOCK_T/_LOCK_RECURSIVE_T as a pointer to an OSMutex
    OSMutex supports recursion automatically
*/

#include <common.h>
#include <wii/os.h>

#include <sys/lock.h>


extern "C" {

/*
    Replacement for OSTryLockMutex, which isn't linked in the game
*/
static int tryLockMutex(wii::os::OSMutex * mutex)
{
    u32 interrupts = wii::os::OSDisableInterrupts();

    // Check whether mutex is free
    int canLock = mutex->count == 0
                || mutex->thread == wii::os::OSGetCurrentThread();
    
    if (canLock)
        wii::os::OSLockMutex(mutex);

    wii::os::OSRestoreInterrupts(interrupts);

    return canLock;
}

/*
    Get/set the mutex represented by a _LOCK_T
*/
static void setLockMutex(_LOCK_T * lock, wii::os::OSMutex * mutex)
{
    *lock = reinterpret_cast<_LOCK_T>(mutex);
}
static wii::os::OSMutex * getLockMutex(_LOCK_T * lock)
{
    return reinterpret_cast<wii::os::OSMutex *>(*lock);
}

/*
    Convert a _LOCK_RECURSIVE_T to a _LOCK_T
*/
static _LOCK_T * fromRecursive(_LOCK_RECURSIVE_T * lock)
{
    return reinterpret_cast<_LOCK_T *>(lock);
}

/*
    Initialise a mutex on the heap
*/
static wii::os::OSMutex * createMutex()
{
    wii::os::OSMutex * mutex = new wii::os::OSMutex();
    wii::os::OSInitMutex(mutex);
    return mutex;
}

/*
    Destroy a mutex on the heap
*/
static void destroyMutex(wii::os::OSMutex * mutex)
{
    delete mutex;
}

void __syscall_lock_init(_LOCK_T * lock)
{
    wii::os::OSMutex * mutex = createMutex();
    setLockMutex(lock, mutex);
}

void __syscall_lock_acquire(_LOCK_T * lock)
{
    wii::os::OSMutex * mutex = getLockMutex(lock);
    wii::os::OSLockMutex(mutex);
}

int  __syscall_lock_try_acquire(_LOCK_T * lock)
{
    wii::os::OSMutex * mutex = getLockMutex(lock);
    return tryLockMutex(mutex);
}

void __syscall_lock_release(_LOCK_T * lock)
{
    wii::os::OSMutex * mutex = getLockMutex(lock);
    wii::os::OSUnlockMutex(mutex);
}

void __syscall_lock_close(_LOCK_T * lock)
{
    wii::os::OSMutex * mutex = getLockMutex(lock);
    destroyMutex(mutex);
    setLockMutex(lock, nullptr);
}

void __syscall_lock_init_recursive(_LOCK_RECURSIVE_T * lockR)
{
    _LOCK_T * lock = fromRecursive(lockR);
    __syscall_lock_init(lock);
}
void __syscall_lock_acquire_recursive(_LOCK_RECURSIVE_T * lockR)
{
    _LOCK_T * lock = fromRecursive(lockR);
    __syscall_lock_acquire(lock);
}

int  __syscall_lock_try_acquire_recursive(_LOCK_RECURSIVE_T * lockR)
{
    _LOCK_T * lock = fromRecursive(lockR);
    return __syscall_lock_try_acquire(lock);
}

void __syscall_lock_release_recursive(_LOCK_RECURSIVE_T * lockR)
{
    _LOCK_T * lock = fromRecursive(lockR);
    __syscall_lock_release(lock);
}

void __syscall_lock_close_recursive(_LOCK_RECURSIVE_T * lockR)
{
    _LOCK_T * lock = fromRecursive(lockR);
    __syscall_lock_close(lock);
}


}
