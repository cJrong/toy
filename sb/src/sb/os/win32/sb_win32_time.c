#include <sb/sb_config.h>
#include <sb/os/win32/sb_win32_time.h>
#include <windows.h>
#include <time.h>

sb_clock_t sb_clock(sb_void_t)
{
    return (sb_clock_t) clock();
}

sb_clock_t sb_clocks_per_sec(sb_void_t)
{
    return (sb_clock_t) CLOCKS_PER_SEC;
}

sb_int_t sb_get_time_zone(sb_void_t)
{
    sb_ulong_t n;
    TIME_ZONE_INFORMATION tz;

    n = GetTimeZoneInformation(&tz);

    switch (n) {

    case TIME_ZONE_ID_UNKNOWN:
        return -tz.Bias;

    case TIME_ZONE_ID_STANDARD:
        return -(tz.Bias + tz.StandardBias);

    case TIME_ZONE_ID_DAYLIGHT:
        return -(tz.Bias + tz.DaylightBias); 
    }

    return 0;
}

sb_bool_t sb_set_system_time(sb_tm_t *tm)
{
    SYSTEMTIME st;

    st.wMilliseconds = tm->milliseconds;
    st.wSecond = tm->second;
    st.wMinute = tm->minute;
    st.wHour = tm->hour;
    st.wDay = tm->day;
    st.wDayOfWeek = tm->week;
    st.wMonth = tm->month;
    st.wYear = tm->year;

    return (sb_bool_t) SetSystemTime(&st);
}

sb_bool_t sb_set_local_time(sb_tm_t *tm)
{
    SYSTEMTIME st;

    st.wMilliseconds = tm->milliseconds;
    st.wSecond = tm->second;
    st.wMinute = tm->minute;
    st.wHour = tm->hour;
    st.wDay = tm->day;
    st.wDayOfWeek = tm->week;
    st.wMonth = tm->month;
    st.wYear = tm->year;

    return (sb_bool_t) SetLocalTime(&st);
}

sb_void_t sb_get_system_time(sb_tm_t *tm)
{
    SYSTEMTIME st;

    GetSystemTime(&st);
    
    tm->milliseconds = st.wMilliseconds;
    tm->second = st.wSecond;
    tm->minute = st.wMinute;
    tm->hour = st.wHour;
    tm->day = st.wDay;
    tm->week = st.wDayOfWeek;
    tm->month = st.wMonth;
    tm->year = st.wYear;
}

sb_void_t sb_get_local_time(sb_tm_t *tm)
{
    SYSTEMTIME st;

    GetLocalTime(&st);

    tm->milliseconds = st.wMilliseconds;
    tm->second = st.wSecond;
    tm->minute = st.wMinute;
    tm->hour = st.wHour;
    tm->day = st.wDay;
    tm->week = st.wDayOfWeek;
    tm->month = st.wMonth;
    tm->year = st.wYear;
}


sb_char_t* sb_strftm(sb_char_t *buf, sb_size_t *len, const sb_char_t *format, 
    const sb_tm_t *tm)
{
    struct tm t;

    t.tm_year   = tm->year - 1900;
    t.tm_mon    = tm->month - 1;
    t.tm_mday   = tm->day;
    t.tm_hour   = tm->hour;
    t.tm_min    = tm->minute;
    t.tm_sec    = tm->second;
    t.tm_wday   = tm->week;

    strftime(buf, *len, format, &t);

    return buf;
}


sb_char_t* sb_cstrftm(sb_char_t *buf, sb_size_t *len, const sb_char_t *format)
{
    sb_tm_t tm;

    sb_get_local_time(&tm);

    return sb_strftm(buf, len, format, &tm);
}


sb_time_t sb_time(sb_time_t *t)
{
    return time(t);
}