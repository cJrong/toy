

#ifndef __SB__STROPS__H__INCLUDE__
#define __SB__STROPS__H__INCLUDE__

#include <sb/sb_config.h>

sb_char_t *sb_strcat(sb_char_t *dst, const sb_char_t *src);
sb_char_t *sb_strncat(sb_char_t *dst, const sb_char_t *src, sb_size_t n);
sb_int_t sb_strcmp(const sb_char_t *s1, const sb_char_t *s2);
sb_int_t sb_strncmp(const sb_char_t *s1, const sb_char_t *s2, sb_size_t n);
sb_char_t *sb_strchr(const sb_char_t *s, sb_int_t ch);
sb_char_t *sb_strstr(const sb_char_t *s, const sb_char_t *pattern);
sb_size_t sb_strlen(const sb_char_t *s);
sb_int_t sb_strcount(const sb_char_t *s, sb_size_t n,
    const sb_char_t *sep, sb_size_t sep_n);

sb_char_t *sb_strfind(const sb_char_t *s, sb_size_t n,
    const sb_char_t *sep, sb_size_t sep_n);
sb_char_t *sb_strrpl(sb_char_t *dst, sb_size_t dst_n,
    const sb_char_t *src, sb_size_t src_n,
    const sb_char_t *from, sb_size_t from_n,
    const sb_char_t *to, sb_size_t to_n,
    sb_int_t max);

#endif
