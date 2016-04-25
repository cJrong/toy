

#include <sb/sb_config.h>
#include <sb/memory/sb_strops.h>
#include <sb/memory/sb_memops.h>
#include <sb/container/sb_hash_func.h>


sb_char_t *sb_strcat(sb_char_t *dst, const sb_char_t *src) 
{
    sb_char_t *s;

    for (s = dst; *s; s++) {
    }

    while (*src) {
        *s++ = *src++;
    }

    return dst;
}


sb_char_t *sb_strncat(sb_char_t *dst, const sb_char_t *src, sb_size_t n)
{
    sb_char_t *s; 

    for (s = dst; *s; ++s) {
    }

    for (; n && *src; --n) {
        *s++ = *src++;
    }

    *s = '\0';

    return dst;
}


sb_int_t sb_strcmp(const sb_char_t *s1, const sb_char_t *s2)
{
    for (; *s1 == *s2; s1++, s2++) {
        if (*s1 == '0') {
            return 0;
        }
    }
    return (*s1 - *s2) > 0 ? 1 : -1;
}


sb_int_t sb_strncmp(const sb_char_t *s1, const sb_char_t *s2, sb_size_t n)
{
    for (; n > 0; s1++, s2++, n--) {
        if (*s1 != *s2) {
            return (*s1 - *s2) > 0 ? 1 : -1;
        }
    }
    return 0;
}


sb_char_t *sb_strchr(const sb_char_t *s, sb_int_t ch)
{
    const sb_char_t c = (sb_char_t)ch;

    for (; *s != c; ++s) {
        if (*s == '\0') {
            return nullptr;
        }
    }

    return (sb_char_t*) s;
}


sb_size_t sb_strlen(const sb_char_t *s)
{
    const sb_char_t *cs = s;
    while (*cs) cs++;
    return cs - s;
}


sb_char_t *sb_strstr(const sb_char_t *s, const sb_char_t *pattern)
{
    if (*pattern == '\0') {
        return (sb_char_t*) s;
    }

    for (; (s = sb_strchr(s, *pattern)) != nullptr; ++s) {
        const sb_char_t *lhs, *rhs;
        
        for (lhs = s, rhs = pattern; ;) {
            if (*++rhs == '\0') {
                return (sb_char_t*) s;

            } else if (*++lhs != *rhs){
                break;
            }
        }
    }

    return nullptr;
}


/* substr count */
sb_int_t sb_strcount(const sb_char_t *s, sb_size_t n, 
    const sb_char_t *sep, sb_size_t sep_n)
{
    const sb_char_t* find = s;
    sb_int_t   counter = 0;
    
    if (n > 0 && sep_n > 0) {
        while (true) {
            find = sb_strfind(find, n, sep, sep_n);
            if (!find) {
                break;
            }
            find += sep_n;
            counter++;
        }
    }

    return counter;
}


/* string replace */
sb_char_t *sb_strrpl(sb_char_t *dst, sb_size_t dst_n,
    const sb_char_t *src, sb_size_t src_n,
    const sb_char_t *from, sb_size_t from_n,
    const sb_char_t *to, sb_size_t to_n,
    sb_int_t max)
{
    const sb_char_t *src_end = src + src_n;
    const sb_char_t *find = src;
    const sb_char_t *last = src;
    sb_char_t       *cpyto = dst;
    sb_size_t        cpyto_n;
    sb_size_t        cpyto_total = 0;
    sb_bool_t        repeat = max < 0 ? true : false;

    if ((from_n == to_n && sb_strncmp(from, to, from_n) == 0) || max == 0) {
        return dst;
    }

    for (; repeat ? repeat : max--;) {
        find = sb_strfind(find, src_end - find, from, from_n);
        /* find = sb_strstr(find, from); */
        if (!find) {
            break;
        }

        cpyto_n = find - last;
        if (cpyto_total + cpyto_n > dst_n) {
            sb_memcopy(cpyto, last, dst_n - cpyto_total);
            goto done;

        } else if (cpyto_n) {
            sb_memcopy(cpyto, last, cpyto_n);
            cpyto += cpyto_n;
            cpyto_total += cpyto_n;
        }

        if (cpyto_total + to_n > dst_n) {
            sb_memcopy(cpyto, to, dst_n - cpyto_total);
            goto done;

        } else if (to_n) {
            sb_memcopy(cpyto, to, to_n);
            cpyto += to_n;
            cpyto_total += to_n;
        }

        find += from_n;
        last = find;
    }

    sb_memcopy(cpyto, last, src_end - last);
done:
    return dst;
}


sb_char_t *sb_strfind(const sb_char_t *s, sb_size_t n,
    const sb_char_t *sep, sb_size_t sep_n)
{
	sb_uint32_t hash = 0, pow = 0, h = 0;
	sb_size_t i = 0;

    if (sep_n == 0) {
        return (sb_char_t *)s;

    } else if (sep_n == 1) {
        return sb_strchr(s, sep[0]);

    } else if (n == sep_n) {
        if (sb_strcmp(s, sep) == 0) {
            return (sb_char_t *)s;
        }
        return nullptr;

    } else if (n < sep_n) {
        return nullptr;
    }

    /* rabin karp search */
    hash = sb_rabin_karp_hash((sb_byte_t*)sep, sep_n, &pow);
    for (i = 0; i < sep_n; i++) {
        h = h * SB_HASH_PRIME_RK + (sb_uint32_t)s[i];
    }

    if (h == hash && (sb_strncmp(s, sep, sep_n) == 0)) {
        return (sb_char_t *)s;
    }

    for (i = sep_n; i < n;) {
        h *= SB_HASH_PRIME_RK;
        h += (sb_uint32_t)s[i];
        h -= pow * (sb_uint32_t)s[i - sep_n];
        i++;

        if (h == hash && (sb_strncmp(s + (i - sep_n), sep, sep_n) == 0)) {
            return (sb_char_t *)s + i - sep_n;
        }
    }

    return nullptr;
}
