#include <common.h>
#include <spm/system.h>
#include <wii/os.h>

#include <sys/iosupport.h>
#include <stdlib.h>

#include "reent.h"
#include "../patch.h"

namespace clibcompat {

/*
    On boot:
        Automatic thread safety, nothing else will be calling into this rel
        Disable interrupts
            Create an entry for all active threads
            Hook OSCreate/ExitThread for future ones
    
    On thread create:
        Malloc reent + entry (the same thread can't get here twice, malloc needs interrupts on)
        Disable interrupts
            Insert into list
    
    On thread exit:
        Disable interrupts
            Remove from list
        Free
    
    On getreent:
        Disable interrupts
        Check list
        Return global if none
*/

struct ListEntry
{
    wii::os::OSThread * thread;
    struct _reent * reent;
    ListEntry * next;
    ListEntry * prev;
};

static ListEntry * list;

static ListEntry * listSearch(wii::os::OSThread * thread)
{
    for (ListEntry * entry = list; entry != nullptr; entry = entry->next)
    {
        if (entry->thread == thread)
            return entry;
    }

    return nullptr;
}

static void registerThread(wii::os::OSThread * thread)
{
    wii::os::OSReport("Register %p\n", thread);

    struct _reent * reent = (struct _reent *) malloc(sizeof(struct _reent));
    _REENT_INIT_PTR(reent);

    ListEntry * entry = (ListEntry *) malloc(sizeof(ListEntry));
    entry->thread = thread;
    entry->reent = reent;

    u32 intr = wii::os::OSDisableInterrupts();

    // Add entry to list
    entry->next = list;
    if (list != nullptr)
        list->prev = entry;
    list = entry;
    entry->prev = nullptr;

    wii::os::OSRestoreInterrupts(intr);
}

static void removeThread(wii::os::OSThread * thread)
{
    wii::os::OSReport("Remove %p\n", thread);

    u32 intr = wii::os::OSDisableInterrupts();

    ListEntry * entry = listSearch(thread);

    // Remove entry from list
    if (entry->prev == nullptr)
        list = entry->next;
    else
        entry->prev->next = entry->next;

    wii::os::OSRestoreInterrupts(intr);

    _reclaim_reent(entry->reent);
    free(entry->reent);
    free(entry);
}

extern "C" struct _reent* __syscall_getreent()
{
    wii::os::OSThread * thread = wii::os::OSGetCurrentThread();

    u32 intr = wii::os::OSDisableInterrupts();

    ListEntry * entry = listSearch(thread);

    wii::os::OSRestoreInterrupts(intr);

    // Use global reent while creating/freeing entry for this thread
    if (entry == nullptr)
        return _GLOBAL_REENT;
    else
        return entry->reent;
}

Trampoline OSCreateThread_trampoline;
Trampoline OSExitThread_trampoline;
Trampoline OSCancelThread_trampoline;

static void (*OSCancelThread_original)(wii::os::OSThread * thread);
static void (*OSExitThread_original)(void * val);

static void onExitThread(void * val)
{
    wii::os::OSThread * thread = wii::os::OSGetCurrentThread();
    
    removeThread(thread);

    OSExitThread_original(val);
}

static void onCancelThread(wii::os::OSThread * thread)
{
    removeThread(thread);

    OSCancelThread_original(thread);
}

static BOOL onCreateThread(BOOL ret)
{
    if (ret)
    {
        wii::os::OSThread * thread = wii::os::OSGetCurrentThread();

        registerThread(thread);
    }

    return ret;
}


void reentPatch()
{
    u32 intr = wii::os::OSDisableInterrupts();

    // Create reents for existing threads
    for (wii::os::OSThread * thread = wii::os::OS_THREAD_QUEUE.head; thread != nullptr; thread = thread->linkActive.next)
        registerThread(thread);

    wii::os::OSRestoreInterrupts(intr);

    // TODO: recursive hooking
    writeBranch(wii::os::OSCreateThread, 0x268, onCreateThread);
    OSCancelThread_original = hookFunction(wii::os::OSCancelThread, onCancelThread, OSCancelThread_trampoline);
    OSExitThread_original = hookFunction(wii::os::OSExitThread, onExitThread, OSExitThread_trampoline);
}

}
