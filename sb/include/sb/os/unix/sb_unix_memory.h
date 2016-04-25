

#ifndef __SB__UNIX__MEMORY__H__INCLUDE__
#define __SB__UNIX__MEMORY__H__INCLUDE__

#include <sb/sb_config.h>

#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

static sb_inline
sb_void_t *__sb_align_alloc__(sb_size_t alignment, sb_size_t size)
{
    return memalign(alignment, size);
}

static sb_inline
sb_void_t __sb_align_free__(sb_void_t *ptr)
{
    free(ptr);
}

static sb_inline
sb_uint_t sb_get_page_size(sb_void_t)
{
    return sysconf(_SC_PAGESIZE);
}

#define sb_alloc            malloc
#define sb_ralloc           realloc
#define sb_zalloc           calloc
#define sb_free             free


#define sb_align_alloc(align, size) __sb_align_alloc__(align, size)
#define sb_align_free(ptr)          __sb_align_free__(ptr)


#endif
