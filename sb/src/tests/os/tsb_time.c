

#include <sb/os/sb_time.h>
#include <stdio.h>
#include <sys/time.h>

int main(void)
{
    sb_tm_t tm;
    sb_char_t buf[1024];
    sb_size_t len = sizeof(buf);

    sb_get_local_time(&tm);

    printf("%s\n", sb_strftm(buf, &len, "%Y-%m-%d %H:%M:%S", &tm));

    sb_get_system_time(&tm);
    printf("%s\n", sb_strftm(buf, &len, "%Y-%m-%d %H:%M:%S", &tm));

    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        printf("%ld,%ld", tv.tv_sec, tv.tv_usec);
    }
    return 0;
}
