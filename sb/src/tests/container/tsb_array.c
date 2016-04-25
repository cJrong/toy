

#include <sb/container/sb_array.h>
#include <sb/io/sb_console.h>
#include <sb/memory/sb_alloc.h>


sb_bool_t magic(sb_void_t *ptr)
{
    *(sb_int_t*)ptr *= 2;
    return true;
}


int main(void)
{
    sb_int_t i;
    sb_int_t *base;
    sb_array_t *v = sb_array_create(sizeof(int));
    for (i = 0; i < 100; i++) {
        sb_int_t *p = sb_array_push(v);
        *p = i;
    }

    for (; i < 200; i++) {
        sb_array_insert(v, i, &i);
    }

    sb_array_shrink(v);

    {
        sb_int_t arr[10];

        for (i = 9; i >= 0; i--) {
            arr[i] = i;
        }

        sb_array_insert_n(v, 14, arr, 10);
    }

    {
        sb_array_t *dst = sb_array_dup(v);

        sb_array_magic(dst, magic);

        sb_array_swap(v, dst);

        sb_array_for_each(dst, base, i) {
            printf("%d, %d\n",i, base[i]);
        }

        sb_array_for_each(v, base, i) {
            printf("%d, %d\n",i, base[i]);
        }

        sb_array_destroy(dst);
    }

    sb_array_destroy(v);
    return 0;
}
