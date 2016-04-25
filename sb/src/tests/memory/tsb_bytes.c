

#include <sb/sb_config.h>
#include <sb/memory/sb_bytes.h>

#include <stdio.h>


void bytes_print(sb_bytes_t *bytes)
{
    sb_uint_t i;
    for (i = 0; i < sb_bytes_readable(bytes); i++) {
        putchar(sb_bytes_peek(bytes)[i]);
    }

    printf("\n");
}

void byte_print(sb_byte_t *buf, sb_int_t size)
{
    sb_int_t i;
    for (i = 0; i < size; i++) {
        putchar(buf[i]);
    }

    printf("\n");
}

int main(void)
{
    sb_bytes_t *bytes;
    sb_byte_t buffer[12];
    sb_int_t size;

    bytes = sb_bytes_create(10);

    sb_bytes_write(bytes, (sb_byte_t*)"Hello World", 12);

    size = sb_bytes_range(bytes, -5, -12, buffer, sizeof(buffer));

    sb_bytes_truncate(bytes, -1);

    sb_bytes_shrink(bytes);

    /*byte_print(buffer, size);
    printf("%d\n", size);*/
    bytes_print(bytes);

    sb_bytes_destroy(bytes);
    return 0;
}
