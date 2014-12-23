#include "libc.h"
#include <stdio.h>
#include <memory>

extern "C"
{
	decltype(&malloc) _cc_alloc = nullptr;
	decltype(&free) _cc_free = nullptr;
	decltype(&realloc) _cc_realloc = nullptr;
	decltype(&vsnprintf_wrap) _cc_vsnprintf = nullptr;
	print_t _cc_print = nullptr;
};

void
set_imports(
	__in decltype(&malloc) cc_alloc,
	__in decltype(&free) cc_free,
	__in decltype(&realloc) cc_realloc,
	__in decltype(&vsnprintf_wrap) cc_vsnprintf,
	__in print_t cc_print
	)
{
	_cc_alloc = cc_alloc;
	_cc_free = cc_free;
	_cc_realloc = cc_realloc;
	_cc_vsnprintf = cc_vsnprintf;
	_cc_print = cc_print;
}

void*
__cdecl
operator new(
	__in size_t size
	)
{
	return malloc(size);
}

void
__cdecl
operator delete(
	__inout void* ptr
	)
{
	if (ptr)
		free(ptr);
}

int 
sprintf(
	__inout_bcount(size) char* buff,
	__in size_t size,
	__in const char* fmt,
	...
	)
{
	va_list arg_list;

	va_start(arg_list, fmt);
	int ret = vsnprintf_wrap(buff, size, fmt, arg_list);
	va_end(arg_list);
	return ret;
}

int
__cdecl 
printf(
	__in const char* fmt,
	...
	)
{
	char buff[0x100] = { 0 };
	va_list arg_list;

	va_start(arg_list, fmt);
	int ret = vsnprintf_wrap(buff, sizeof(buff), fmt, arg_list);
	va_end(arg_list);
	if (ret)
		_cc_print(buff);
	return ret;
}
