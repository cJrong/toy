

#ifndef __SB__WIN32__MEMORY__H__INCLUDE__
#define __SB__WIN32__MEMORY__H__INCLUDE__


#include <sb/sb_config.h>
#include <sb/os/win32/sb_win32_config.h>

#include <malloc.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>

static sb_inline
sb_void_t *__sb_align_alloc__(sb_ulong_t alignment, sb_ulong_t size)
{
    if (((alignment > 0) && ((alignment & alignment - 1)) == 0)) {
        return _aligned_malloc(size, alignment);
    }

    return nullptr;
}

static sb_inline 
sb_void_t __sb_align_free__(sb_void_t *ptr)
{
    _aligned_free(ptr);
}

static sb_inline 
sb_uint_t sb_get_page_size(sb_void_t)
{
    SYSTEM_INFO si = {0};
    GetSystemInfo(&si);
    return si.dwPageSize;
}

#define sb_alloc                        malloc 
#define sb_ralloc                       realloc
#define sb_calloc                       calloc
#define sb_free                         free
#define sb_align_alloc                  __sb_align_alloc__
#define sb_align_free                   __sb_align_free__

#endif