#pragma once

#define _CRT_ALLOCATION_DEFINED

#include <sal.h>
#include <stdarg.h>
#include <stdio.h>

extern "C"
void*
__cdecl
malloc(
	__in size_t size
	);

extern "C"
void
__cdecl
free(
	__inout void* ptr
	);

extern "C"
void*
__cdecl
realloc(
	__inout_opt void* ptr,
	__in size_t size
	);

extern "C"
void*
__cdecl 
calloc(
	__in size_t n, 
	__in size_t size
	); 

extern "C"
int
__cdecl
vsnprintf_wrap(
	__in_ecount(count) char *buf,
	__in size_t count,
	__in const char *fmt,
	__in va_list args
	);

typedef void(_cdecl*print_t)(const char*);

void
set_imports(
	__in decltype(&malloc) cc_alloc,
	__in decltype(&free) cc_free,
	__in decltype(&realloc) cc_realloc,
	__in decltype(&vsnprintf_wrap) cc_vsnprintf,
	__in print_t cc_print
	);
