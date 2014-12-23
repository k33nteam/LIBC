#include "pool.h"
#include <memory>

#pragma warning(push)               
#pragma warning (disable : 4565)
__drv_maxIRQL(DISPATCH_LEVEL)
void* 
__cdecl 
PoolAlloc(
	__in size_t size
	)
{
	if (MAXSIZE_T - sizeof(MEMBLOCK) < size)
		return nullptr;

	MEMBLOCK* block = static_cast<MEMBLOCK*>(
		ExAllocatePoolWithTag(
			NonPagedPoolNxCacheAligned, 
			size + sizeof(MEMBLOCK), 
			_LIBC_POOL_TAG));

	if (nullptr == block)
		return nullptr;
	block->size = size;	
	return block->data;
}

__drv_maxIRQL(DISPATCH_LEVEL)
void 
__cdecl 
PoolFree(
	__inout void* ptr
	)
{
	if (ptr)
		ExFreePoolWithTag(CONTAINING_RECORD(ptr, MEMBLOCK, data), _LIBC_POOL_TAG);
}

__drv_maxIRQL(DISPATCH_LEVEL)
void* 
__cdecl 
PoolReAlloc(
	__in_opt void* ptr, 
	__in size_t size
	)
{
	if (!ptr)
		return PoolAlloc(size);

	if (CONTAINING_RECORD(ptr, MEMBLOCK, data)->size >= size)
		return ptr;

	auto inblock = std::unique_ptr<unsigned char>(static_cast<unsigned char*>(ptr));

	// alloc new block
	void* mem = PoolAlloc(size);
	if (!mem)
		return nullptr;

	// copy from old one, not overflow ..
	memcpy(mem, inblock.get(), size);
	return mem;
}

__drv_maxIRQL(DISPATCH_LEVEL)
int
__cdecl 
PoolVsnPrintf(
	__in_ecount(count) char *buf,
	__in size_t count,
	__in const char *fmt,
	__in va_list args
	)
{
	return 
#ifdef _WIN64
		vsprintf_s(buf, count, fmt, args);
#else
		_vsnprintf(buf, count, fmt, args);
#endif
}

__drv_maxIRQL(DISPATCH_LEVEL)
void
__cdecl 
PoolPrint(
	__in const char *buf
	)
{
	DbgPrint(buf);
}

#pragma warning(pop)
