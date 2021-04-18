#include <spm/memory.h>

void *operator new(std::size_t size)
{
	return spm::memory::__memAlloc(0, size);
}

void *operator new[](std::size_t size)
{
	return spm::memory::__memAlloc(0, size);
}

void operator delete(void *ptr)
{
	return spm::memory::__memFree(0, ptr);
}

void operator delete[](void *ptr)
{
	return spm::memory::__memFree(0, ptr);
}

void operator delete(void *ptr, std::size_t size)
{
	(void) size;
	return spm::memory::__memFree(0, ptr);
}

void operator delete[](void *ptr, std::size_t size)
{
	(void) size;
	return spm::memory::__memFree(0, ptr);
}
