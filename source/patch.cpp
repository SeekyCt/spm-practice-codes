#include "patch.h"

#include <types.h>
#include <wii/OSCache.h>

namespace mod::patch {

void clear_DC_IC_Cache(void * ptr, u32 size)
{
	wii::OSCache::DCFlushRange(ptr, size);
	wii::OSCache::ICInvalidateRange(ptr, size);
}

void writeBranch(void * ptr, void * destination, bool link)
{
	u32 delta = reinterpret_cast<u32>(destination) - reinterpret_cast<u32>(ptr);
	u32 value = link ? 0x48000001 : 0x48000000;
	value |= (delta & 0x03FFFFFC);
	
	u32 * p = reinterpret_cast<u32 *>(ptr);
	*p = value;

	clear_DC_IC_Cache(ptr, sizeof(u32));
}

}