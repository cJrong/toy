
#include <sb/sb_config.h>
#include <sb/memory/sb_memops.h>

#ifndef SB_USE_ANSI_LIBC
sb_void_t *sb_memchr(const sb_void_t *s, sb_byte_t ch, sb_size_t n)
{
    const sb_byte_t *b = (const sb_bytep_t) s;

    for (; n; --n, b++) {
        if (*b == ch) {
            return (sb_void_t*)b;
        }
    }

    return nullptr;
}


sb_int_t sb_memcmp(const sb_void_t *s1, const sb_void_t *s2, sb_size_t n)
{
    const sb_byte_t *b1 = (const sb_bytep_t) s1;
    const sb_byte_t *b2 = (const sb_bytep_t) s2;

    for (; n; ++b1, ++b2, --n) {
        if (*b1 != *b2) {
            return (*b1 > *b2) ? 1 : -1;
        }
    }

    return 0;
}


sb_void_t *sb_memset(sb_void_t *s, sb_byte_t ch, sb_size_t n)
{
    sb_byte_t *b = (sb_bytep_t) s;

    for (; n; ++b, --n) {
        *b = ch;
    }

    return s;
}


sb_void_t *sb_memcopy(sb_void_t *dst, const sb_void_t *src, sb_size_t n)
{
    sb_byte_t       *to = (sb_bytep_t) dst;
    const sb_byte_t *from = (const sb_bytep_t) src;

    for (; n; ++to, ++from, --n) {
        *to = *from;
    }

    return dst;
}


sb_void_t *sb_memmove(sb_void_t *dst, const sb_void_t *src, sb_size_t n)
{
    sb_byte_t       *to = (sb_bytep_t) dst;
    const sb_byte_t *from = (const sb_bytep_t) src;

    if ((from < to) && (to < from + n)) {
        for (to += n, from += n; n; --n) {
            *--to = *--from;
        }

    } else {
        while (n--) {
            *to++ = *from++;
        }
    }

    return dst;
}
#endif
