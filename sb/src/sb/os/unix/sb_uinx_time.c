

#define _DEFAULT_SOURCE
#include <sb/sb_config.h>
#include <sb/memory/sb_memops.h>
#include <sb/os/unix/sb_unix_time.h>


#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>


sb_void_t sb_get_local_time(sb_tm_t *tm)
{
    struct tm stm;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    localtime_r(&tv.tv_sec, &stm);

    tm->year    = stm.tm_year + 1900;
    tm->month   = stm.tm_mon;
    tm->day     = stm.tm_mday;
    tm->hour    = stm.tm_hour;
    tm->minute  = stm.tm_min;
    tm->second  = stm.tm_sec;
    tm->milliseconds = 0;
}


sb_void_t sb_get_system_time(sb_tm_t *tm)
{
    struct tm stm;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    gmtime_r(&tv.tv_sec, &stm);

    tm->year    = stm.tm_year + 1900;
    tm->month   = stm.tm_mon;
    tm->day     = stm.tm_mday;
    tm->hour    = stm.tm_hour;
    tm->minute  = stm.tm_min;
    tm->second  = stm.tm_sec;
    tm->milliseconds = tv.tv_usec / 1000;
}


sb_char_t* sb_strftm(sb_char_t *buf, sb_size_t *len, const sb_char_t *format,
    const sb_tm_t *tm)
{
    struct tm t;

    t.tm_year   = tm->year - 1900;
    t.tm_mon    = tm->month;
    t.tm_mday   = tm->day;
    t.tm_hour   = tm->hour;
    t.tm_min    = tm->minute;
    t.tm_sec    = tm->second;
    t.tm_wday   = tm->week;

    *len = strftime(buf, *len, format, &t);

    return buf;
}


sb_char_t* sb_cstrftm(sb_char_t *buf, sb_size_t *len, const sb_char_t *format)
{
    struct tm stm;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    localtime_r(&tv.tv_sec, &stm);

    *len = strftime(buf, *len, format, &stm);

    return buf;
}
