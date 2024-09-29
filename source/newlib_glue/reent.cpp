#include <common.h>
#include <wii/os.h>

#include <sys/iosupport.h>
#include <map>

extern "C" {

std::map<wii::os::OSThread *, struct _reent*> reentMap;

struct _reent* __syscall_getreent()
{
    wii::os::OSThread * thread = wii::os::OSGetCurrentThread();

    if (reentMap.count(thread) == 0)
        reentMap[thread] = new struct _reent();
    
    return reentMap[thread];
}

}
