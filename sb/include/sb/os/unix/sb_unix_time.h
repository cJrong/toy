

#ifndef __SB__UNIX__TIME__H__INCLUDE__
#define __SB__UNIX__TIME__H__INCLUDE__

#include <sb/sb_config.h>
#include <time.h>

typedef clock_t sb_clock_t;

#define sb_clock          clock
#define SB_CLOCKS_PER_SEC CLOCKS_PER_SEC

typedef struct sb_tm_s {
    sb_ushort_t milliseconds;
    sb_ushort_t second;
    sb_ushort_t minute;
    sb_ushort_t hour;
    sb_ushort_t day;
    sb_ushort_t month;
    sb_ushort_t year;
    sb_ushort_t week;
}sb_tm_t;

sb_void_t sb_get_local_time(sb_tm_t *tm);
sb_void_t sb_get_system_time(sb_tm_t *tm);

sb_char_t* sb_strftm(sb_char_t *buf, sb_size_t *len, const sb_char_t *format,
    const sb_tm_t *tm);
sb_char_t* sb_cstrftm(sb_char_t *buf, sb_size_t *len, const sb_char_t *format);

#endif
