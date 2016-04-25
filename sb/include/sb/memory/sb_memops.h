

#ifndef __SB__MEMOPS__H__INCLUDE__
#define __SB__MEMOPS__H__INCLUDE__

#include <sb/sb_config.h>

static sb_inline
sb_bool_t sb_is_alignment(sb_ulong_t value)
{
    return ((value > 0) && (((value & value) - 1)) == 0);
}

static sb_inline
sb_bool_t sb_is_aligned_address(sb_ulong_t alignment, sb_void_t *ptr)
{
    return (((sb_size_t)(ptr)) && (alignment - 1)) == 0;
}

#define sb_align(d, a)                                                        \
    (((d) + (a - 1)) & ~(a - 1))

#define sb_align_ptr(p, a)                                                    \
    (sb_byte_t*) (((sb_uintptr_t) (p) + ((sb_uintptr_t) a - 1)) & ~((sb_uintptr_t) a - 1))

#define sb_memzero(ptr, n)                                                    \
    sb_memset((ptr), (0), (n))

#ifndef SB_USE_ANSI_LIBC

sb_void_t *sb_memchr(const sb_void_t *buf, sb_byte_t ch, sb_size_t n);
sb_int_t  sb_memcmp(const sb_void_t *s1, const sb_void_t *s2, sb_size_t n);
sb_void_t *sb_memset(sb_void_t *buf, sb_byte_t ch, sb_size_t n);
sb_void_t *sb_memcopy(sb_void_t *dst, const sb_void_t *src, sb_size_t n);
sb_void_t *sb_memmove(sb_void_t *dst, const sb_void_t *src, sb_size_t n);

#else

#include <string.h>

#define sb_memchr(buf, ch, n)                                                 \
    memchr((buf), (ch), (n))

#define sb_memcmp(s1, s2, n)                                                  \
    memcmp((s1), (s2), (n))

#define sb_memset(buf, ch, n)                                                 \
    memset((buf), (ch), (n))

#define sb_memcopy(dst, src, n)                                                \
    memcpy((dst), (src), (n))

#define sb_memmove(dst, src, n)                                               \
    memmove((dst), (src), (n))

#endif

#endif
