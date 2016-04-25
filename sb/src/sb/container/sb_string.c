

#include <sb/sb_config.h>
#include <sb/traits/sb_args.h>
#include <sb/memory/sb_alloc.h>
#include <sb/memory/sb_memops.h>
#include <sb/memory/sb_strops.h>
#include <sb/container/sb_string.h>
#include <sb/container/sb_hash_func.h>

#ifndef SB_STR_MAX_EXPAND_LIMIT
#define SB_STR_MAX_EXPAND_LIMIT     (1024*1024) 
#endif

#ifndef SB_STR_MAX_INTEGER_SIZE
#define SB_STRING_MAX_INTEGER_SIZE  (64)
#endif

static const sb_char_t __sb_to_digit__[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

static sb_inline
sb_string_t __sb_string_expand__(sb_string_t str, sb_size_t addlen);
static
sb_string_t __sb_string_replace__(sb_string_t str,
    const sb_char_t *pattern, sb_size_t pattern_len,
    const sb_char_t *to, sb_size_t to_len, sb_int_t max);


sb_string_t sb_string_create(const sb_char_t *s)
{
    sb_size_t n = s == nullptr ? 0 : sb_strlen(s);
    return sb_string_create_n(s, n);
}


sb_string_t sb_string_create_n(const sb_char_t *s, sb_size_t n)
{
    sb_strreal_t *sr;

    sr = (sb_strreal_t *) sb_alloc(sizeof(sb_strreal_t) + n);
    if (!sr) {
        return nullptr;
    }

    sr->length = n;
    sr->free   = 0;

    if (n && s) {
        sb_memcopy(sr->buffer, s, n);
    }

    sr->buffer[n] = '\0';

    return (sb_string_t) sr->buffer;
}


sb_void_t sb_string_destroy(sb_string_t str)
{
    if (str) {
        sb_free(sb_string_of(str));
    }
}


sb_void_t sb_string_adjust(sb_string_t str)
{
    sb_strreal_t *sr = sb_string_of(str);
    sb_size_t len = sb_strlen(str);

    sr->free += (sr->length - len);
    sr->length = len;
}


sb_string_t sb_string_shrink(sb_string_t str)
{
    sb_strreal_t *sr = sb_string_of(str);

    sr = (sb_strreal_t *)sb_ralloc(sr, sizeof(sb_strreal_t) + sr->length);

    sr->free = 0;

    return (sb_string_t) sr->buffer;
}


sb_string_t sb_string_reserve(sb_string_t str, sb_size_t addlen)
{
    sb_strreal_t *sr = sb_string_of(str);
    sb_size_t total = sr->length + sr->free;

    if (sr->free >= addlen) {
        return sr->buffer;
    }

    sr = (sb_strreal_t*)sb_ralloc(sr,
        sizeof(sb_strreal_t) + total + (addlen - sr->free));
    if (!sr) {
        return nullptr;
    }

    sr->free = addlen;

    return (sb_string_t) sr->buffer;
}


sb_string_t sb_string_resize(sb_string_t str, sb_size_t n)
{
    sb_strreal_t *sr = sb_string_of(str);
    sb_size_t total = sr->length + sr->free;

    if (total > n) {
        sr->free = total - n;
        sr->length = n;
        sr->buffer[n] = '\0';
        return sr->buffer;
    }
     
    str = sb_string_reserve(str, n - sr->length);
    if (!str) {
        return nullptr;
    }

    sr = sb_string_of(str);

    sb_memzero(&sr->buffer[sr->length], n - sr->length);

    sr->length = n;
    sr->free   = 0;

    return (sb_string_t) sr->buffer;
}


sb_string_t sb_string_cat_n(sb_string_t str, const sb_char_t *s, sb_size_t n)
{
    sb_strreal_t *sr;

    str = __sb_string_expand__(str, n);
    if (!str) {
        return nullptr;
    }

    sr = sb_string_of(str);

    sb_memcopy(str + sr->length, s, n);

    sr->length += n;
    sr->free -= n;
    sr->buffer[sr->length] = '\0';

    return str;
}


sb_string_t sb_string_cat(sb_string_t str, const sb_char_t *s)
{
    return sb_string_cat_n(str, s, sb_strlen(s));
}


sb_string_t sb_string_cat_s(sb_string_t dst, const sb_string_t src)
{
    return sb_string_cat_n(dst, src, sb_string_length(src));
}


sb_string_t sb_string_copy_n(sb_string_t str, const sb_char_t *s, sb_size_t n)
{
    sb_strreal_t *sr = sb_string_of(str);
    sb_size_t total = sr->free + sr->length;

    if (total < n) {
        str = __sb_string_expand__(str, n - total);
        if (!str) {
            return nullptr;
        }

        sr = sb_string_of(str);
        total = sr->free + sr->length;
    }

    sb_memcopy(str, s, n);

    sr->buffer[n] = '\0';
    sr->length = n;
    sr->free = total - n;

    return str;
}


sb_string_t sb_string_copy(sb_string_t str, const sb_char_t *s)
{
    return sb_string_copy_n(str, s, sb_strlen(s));
}


sb_string_t sb_string_copy_s(sb_string_t dst, const sb_string_t src)
{
    return sb_string_copy_n(dst, src, sb_string_length(src));
}


sb_string_t sb_string_range(sb_string_t str, sb_ssize_t start, sb_ssize_t end)
{
    sb_strreal_t *sr = sb_string_of(str);
    sb_size_t total = sr->length + sr->free;
    sb_size_t newlen;

    if (sr->length == 0) {
        return (sb_string_t) sr->buffer;
    }

    start = start >= 0 ? start : 0;
    end = end >= 0 ? end : sr->length - 1;
    newlen = (sb_size_t)(start > end ? 0 : end - start + 1);
    newlen = sr->length > newlen ? newlen : sr->length;

    if (start && newlen) {
        sb_memmove(sr->buffer, (sb_bytep_t)sr->buffer + start, newlen);
    }

    sr->length = newlen;
    sr->free = total - newlen;
    sr->buffer[newlen] = '\0';

    return (sb_string_t) sr->buffer;
}


sb_int_t sb_string_cmp(sb_string_t lhs, sb_string_t rhs)
{
    sb_size_t lhs_len = sb_string_length(lhs);
    sb_size_t rhs_len = sb_string_length(rhs);
    sb_int_t min_len = lhs_len < rhs_len ? lhs_len : rhs_len;
    sb_int_t cmp;

    cmp = sb_memcmp(lhs, rhs, min_len);

    if (cmp == 0) {
        return lhs_len - rhs_len;
    }

    return cmp;
}


sb_void_t sb_string_tolower(sb_string_t str)
{
    sb_int_t len = sb_string_length(str);
    sb_int_t i;
    
    for (i = 0; i < len; i++) {
        str[i] |= 0x20; 
    }
}


sb_void_t sb_string_toupper(sb_string_t str)
{
    sb_int_t len = sb_string_length(str);
    sb_int_t i;
    
    for (i = 0; i < len; i++) {
        str[i] &= 0xDF;
    }
}


sb_string_t sb_string_trim(sb_string_t str, const sb_char_t *chset)
{
    sb_strreal_t *sr = sb_string_of(str);
    sb_size_t  n = sr->length;
    sb_char_t *p = str;

    sr->free  += sr->length;
    sr->length = 0;
    
    for (; n-- ; p++) {
        if (!sb_strchr(chset, *p)) {
            sb_string_cat_n(str, p, 1);
        }
    }

    return str;
}


sb_string_t sb_string_fmt(sb_string_t str, const sb_char_t *fmt, ...)
{
    sb_strreal_t *sr = sb_string_of(str);
    sb_size_t n = sb_string_length(str);
    const sb_char_t *format;
    sb_int_t i;
    sb_vargs_t ap;

    sb_vargs_start(ap, fmt);

    format = fmt;
    i = n;

    while (*format) {
        sb_size_t len;

        /* args... */
        sb_char_t *s;

        if (sr->free == 0) {
            str = __sb_string_expand__(str, 1);
            sr = sb_string_of(str);
        }

        switch (*format) {
        case '%':
            switch (*++format) {
            case 's':
            case 'S':
                s = sb_vargs_arg(ap, char*);

                len = (*format == 's') ? sb_strlen(s) : sb_string_length(s);
                if (sr->free < len) {
                    str = __sb_string_expand__(str, len);
                    sr = sb_string_of(str);
                }
                sb_memcopy(str + i, s, len);
                sr->length += len;
                sr->free -= len;
                i += len;
                break;
            default:
                str[i++] = *format;
                sr->length += 1;
                sr->free -=1;
            }
            break;

        default:
            str[i++] = *format;
            sr->length += 1;
            sr->free -= 1;
            break;
        }

        format++;
    }

    sb_vargs_end(ap);

    str[i] = '\0';

    return str;
}


sb_int_t sb_string_index_n(sb_string_t str, const sb_char_t *sep, sb_size_t sep_n)
{
    sb_strreal_t *sr = sb_string_of(str);
    sb_uint32_t   hash = 0, pow = 0, h = 0, i = 0;

    if (sep_n == 0) {
        return 0;

    } else if (sep_n == 1) {
        sb_uint_t i = 0;

        for (; i < sr->length; i++) {
            if (str[i] == sep[0]) {
                return i;
            }
        }
        return -1;

    } else if (sr->length == sep_n) {
        if (sb_strcmp(str, sep) == 0) {
            return 0;
        }
        return -1;

    } else if (sr->length < sep_n) {
        return -1;
    }

    /* rabin karp search */
    hash = sb_rabin_karp_hash((sb_byte_t *)sep, sep_n, &pow);
    for (i = 0; i < sep_n; i++) {
        h = h * SB_HASH_PRIME_RK + (sb_uint32_t)str[i];
    }

    if (h == hash && (sb_strncmp(str, sep, sep_n) == 0)) {
        return 0;
    }

    for (i = sep_n; i < sr->length;) {
        h *= SB_HASH_PRIME_RK;
        h += (sb_uint32_t)str[i];
        h -= pow * (sb_uint32_t)str[i - sep_n];
        i++;

        if (h == hash && (sb_strncmp(str + (i - sep_n), sep, sep_n) == 0)) {
            return i - sep_n;
        }
    }

    return -1;
}


sb_void_t sb_string_swap(sb_string_t *lhs, sb_string_t *rhs)
{
    sb_strreal_t *lhsr = sb_string_of(*lhs);
    sb_strreal_t *rhsr = sb_string_of(*rhs);
    sb_void_t *tmp;

    tmp = lhsr;
    lhsr = rhsr;
    rhsr = tmp;

    *lhs = lhsr->buffer;
    *rhs = rhsr->buffer;
}


sb_string_t sb_string_replace(sb_string_t str, 
    const sb_char_t *pattern, const sb_char_t *to, sb_int_t max)
{
    sb_string_t dst;

    if (sb_strcmp(pattern, to) == 0 || max == 0) {
        return str;
    }

    dst = __sb_string_replace__(str, pattern,
        sb_strlen(pattern), to, sb_strlen(to), max);

    sb_string_destroy(str);

    return dst;
}


sb_string_t sb_string_replace_s(sb_string_t str, 
    const sb_string_t pattern, const sb_string_t to, sb_int_t max)
{
    sb_string_t dst;

    if (sb_string_cmp(pattern, to) == 0 || max == 0) {
        return str;
    }

    dst = __sb_string_replace__(str, pattern,
        sb_string_length(pattern), to, sb_string_length(to), max);

    sb_string_destroy(str);

    return dst;
}


sb_string_t sb_string_from_int(sb_int_t value, sb_uint_t base)
{
    sb_char_t buf[SB_STRING_MAX_INTEGER_SIZE];
    sb_char_t tmp;
    sb_uint_t v;
    sb_int_t i = 0, j = 0, length = 0;

    v = (value < 0) ? -value : value;

    do {
        buf[i++] = __sb_to_digit__[(v) % base];
        v /= base;
    } while (v);

    if (value < 0 && base == 10) {
        buf[i++] = '-';
    }

    buf[i] = '\0';

    for (length = i, i--; i > j; i--, j++) {
        tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }

    return sb_string_create_n(buf, length);
}


sb_string_t sb_string_from_uint(sb_uint_t value, sb_uint_t base)
{
    sb_char_t buf[SB_STRING_MAX_INTEGER_SIZE];
    sb_char_t tmp;
    sb_int_t i = 0, j = 0, length = 0;

    do {
        buf[i++] = __sb_to_digit__[(value % base)];
        value /= base;
    } while (value);

    buf[i] = '\0';

    for (length = i, i--; i > j; i--, j++) {
        tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }

    return sb_string_create_n(buf, length);
}


sb_string_t sb_string_from_long(sb_long_t value, sb_uint_t base)
{
    sb_char_t  buf[SB_STRING_MAX_INTEGER_SIZE];
    sb_char_t  tmp;
    sb_ulong_t v;
    sb_int_t i = 0, j = 0, length = 0;

    v = (value < 0) ? -value : value;

    do {
        buf[i++] = __sb_to_digit__[(v) % base];
        v /= base;
    } while (v);

    if (value < 0 && base == 10) {
        buf[i++] = '-';
    }

    buf[i] = '\0';

    for (length = i, i--; i > j; i--, j++) {
        tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }

    return sb_string_create_n(buf, length);
}


sb_string_t sb_string_from_ulong(sb_ulong_t value, sb_uint_t base)
{
    sb_char_t buf[SB_STRING_MAX_INTEGER_SIZE];
    sb_char_t tmp;
    sb_int_t i = 0, j = 0, length = 0;

    do {
        buf[i++] = __sb_to_digit__[(value % base)];
        value /= base;
    } while (value);

    buf[i] = '\0';

    for (length = i, i--; i > j; i--, j++) {
        tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }

    return sb_string_create_n(buf, length);
}


#ifdef SB_HAVE_LONGLONG
sb_string_t sb_string_from_llong(sb_llong_t value, sb_uint_t base)
{
    sb_char_t   buf[SB_STRING_MAX_INTEGER_SIZE];
    sb_char_t   tmp;
    sb_ullong_t v;
    sb_int_t i = 0, j = 0, length = 0;

    v = (value < 0) ? -value : value;

    do {
        buf[i++] = __sb_to_digit__[(v) % base];
        v /= base;
    } while (v);

    if (value < 0 && base == 10) {
        buf[i++] = '-';
    }

    buf[i] = '\0';

    for (length = i, i--; i > j; i--, j++) {
        tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }

    return sb_string_create_n(buf, length);
}


sb_string_t sb_string_from_ullong(sb_ullong_t value, sb_uint_t base)
{
    sb_char_t buf[SB_STRING_MAX_INTEGER_SIZE];
    sb_char_t tmp;
    sb_int_t i = 0, j = 0, length = 0;

    do {
        buf[i++] = __sb_to_digit__[(value % base)];
        value /= base;
    } while (value);

    buf[i] = '\0';

    for (length = i, i--; i > j; i--, j++) {
        tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }

    return sb_string_create_n(buf, length);
}
#endif


static sb_inline
sb_string_t __sb_string_expand__(sb_string_t str, sb_size_t addlen)
{
    sb_strreal_t *sr, *nsr;
    sb_size_t newlen;

    sr = sb_string_of(str);
    if (sr->free >= addlen) {
        return (sb_string_t)sr->buffer;
    }

    newlen = sr->length + addlen;
    if (newlen <= SB_STR_MAX_EXPAND_LIMIT) {
        newlen *= 2;
    } else {
        newlen += SB_STR_MAX_EXPAND_LIMIT;
    }

    nsr = (sb_strreal_t*)sb_ralloc(sr, sizeof(sb_strreal_t) + newlen);
    if (!nsr) {
        return nullptr;
    }

    nsr->free = newlen - nsr->length;

    return nsr->buffer;
}


static
sb_string_t __sb_string_replace__(sb_string_t str,
    const sb_char_t *pattern, sb_size_t pattern_len,
    const sb_char_t *to, sb_size_t to_len, sb_int_t max)
{
    sb_string_t str_end = str + sb_string_length(str);
    sb_string_t dst = sb_string_create("");
    sb_char_t *find = str;
    sb_char_t *last = str;
    sb_bool_t repeat = max < 0 ? true : false;

    dst = sb_string_reserve(dst, sb_string_length(str));

    for (; repeat ? repeat : max--;) {
        find = sb_strstr(find, pattern);
        if (find == nullptr) {
            goto done;
        }

        dst = sb_string_cat_n(dst, last, find - last);
        dst = sb_string_cat_n(dst, to, to_len);

        find += pattern_len;
        last = find;
    }

done:
    dst = sb_string_cat_n(dst, last, str_end - last);
    return dst;
}
