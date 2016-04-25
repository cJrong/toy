

#ifndef __SB__STRING__H__INCLUDE__
#define __SB__STRING__H__INCLUDE__

#include <sb/sb_config.h>

typedef sb_char_t* sb_string_t;

typedef struct sb_strreal_s{
    sb_size_t  length;
    sb_size_t  free;
    sb_char_t  buffer[1];
}sb_strreal_t;

#define sb_string_of(str)                                                     \
    ((sb_strreal_t*)(((sb_bytep_t)(str)) - (sb_bytep_t)(&(((sb_strreal_t*)0)->buffer))))

#define sb_string_length(str)                                                 \
    sb_string_of((str))->length

#define sb_string_size sb_string_length

#define sb_string_is_empty(str)                                               \
    (sb_string_of((str))->length == 0)

#define sb_string_capacity(str)                                               \
    sb_string_of((str))->free

#define sb_string_empty()                                                     \
    sb_string_create_n("", 0)

#define sb_string_index_s(str, sep)                                           \
    sb_string_index((str), (sep), (sb_string_length(sep)))

#define sb_string_dup(str)                                                    \
    sb_string_create_n((str), sb_string_length(str))

#define sb_string_dup_n(str, len)                                             \
    sb_string_create_n((str), (len))

sb_string_t sb_string_create(const sb_char_t *s);
sb_string_t sb_string_create_n(const sb_char_t *s, sb_size_t n);
sb_void_t sb_string_destroy(sb_string_t str);
sb_string_t sb_string_shrink(sb_string_t str);
sb_string_t sb_string_reserve(sb_string_t str, sb_size_t addlen);
sb_string_t sb_string_resize(sb_string_t str, sb_size_t n);
sb_string_t sb_string_cat(sb_string_t str, const sb_char_t *s);
sb_string_t sb_string_cat_n(sb_string_t str, const sb_char_t *s, sb_size_t n);
sb_string_t sb_string_cat_s(sb_string_t dst, const sb_string_t src);
sb_string_t sb_string_copy(sb_string_t str, const sb_char_t *s);
sb_string_t sb_string_copy_n(sb_string_t str, const sb_char_t *s, sb_size_t n);
sb_string_t sb_string_copy_s(sb_string_t dst, const sb_string_t src);
sb_int_t sb_string_cmp(sb_string_t lhs, sb_string_t rhs);
sb_string_t sb_string_trim(sb_string_t str, const sb_char_t *chset);
sb_void_t sb_string_swap(sb_string_t *lhs, sb_string_t *rhs);
sb_string_t sb_string_fmt(sb_string_t str, const sb_char_t *fmt, ...);
sb_string_t sb_string_range(sb_string_t str, sb_ssize_t start, sb_ssize_t end);

sb_int_t sb_string_index_n(sb_string_t str,
    const sb_char_t *sep, sb_size_t sep_n);

sb_string_t sb_string_replace(sb_string_t str,
    const sb_char_t *pattern, const sb_char_t *to, sb_int_t max);
sb_string_t sb_string_replace_s(sb_string_t str,
    const sb_string_t pattern, const sb_string_t to, sb_int_t max);

sb_string_t sb_string_from_int(sb_int_t value, sb_uint_t base);
sb_string_t sb_string_from_uint(sb_uint_t value, sb_uint_t base);
sb_string_t sb_string_from_long(sb_long_t value, sb_uint_t base);
sb_string_t sb_string_from_ulong(sb_ulong_t value, sb_uint_t base);
#ifdef SB_HAVE_LONGLONG
sb_string_t sb_string_from_llong(sb_llong_t value, sb_uint_t base);
sb_string_t sb_string_from_ullong(sb_ullong_t value, sb_uint_t base);
#endif

static sb_inline
sb_void_t sb_string_clear(sb_string_t str)
{
    sb_strreal_t *sr = sb_string_of(str);
    sr->free     += sr->length;
    sr->length    = 0;
    sr->buffer[0] = '\0';
}

static sb_inline
sb_size_t sb_string_total(sb_string_t str)
{
    sb_strreal_t *sr = sb_string_of(str);
    return sizeof(sb_strreal_t) + sr->length + sr->free;
}

#endif
