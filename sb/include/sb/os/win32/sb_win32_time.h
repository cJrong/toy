

#ifndef __SB__WIN32__TIME__H__INCLUDE__
#define __SB__WIN32__TIME__H__INCLUDE__

#include <sb/sb_config.h>
#include <sb/os/win32/sb_win32_config.h>

typedef sb_long_t sb_clock_t;

sb_clock_t sb_clock(sb_void_t);
sb_clock_t sb_clocks_per_sec(sb_void_t);

typedef sb_int64_t sb_time_t;
sb_time_t sb_time(sb_time_t *t);


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

sb_int_t sb_get_time_zone(sb_void_t);

sb_bool_t sb_set_system_time(sb_tm_t *tm);
sb_void_t sb_get_system_time(sb_tm_t *tm);
sb_bool_t sb_set_local_time(sb_tm_t *tm);
sb_void_t sb_get_local_time(sb_tm_t *tm);
sb_char_t* sb_strftm(sb_char_t *buf, sb_size_t *len, const sb_char_t *format,
    const sb_tm_t *tm);


#endif
