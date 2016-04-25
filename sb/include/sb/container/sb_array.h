

#ifndef __SB__VECTOR__H__INCLUDE__
#define __SB__VECTOR__H__INCLUDE__

#include <sb/sb_config.h>

typedef struct sb_array_s {
    sb_void_t *elts;
    sb_size_t  nelts;
    sb_size_t  size;
    sb_size_t  nalloc;
}sb_array_t;

typedef sb_int_t (*sb_array_compare_pt)(const sb_void_t *, const sb_void_t *);
typedef sb_bool_t(*sb_array_magic_pt)(sb_void_t *);

#define sb_array_is_empty(array)                                              \
    (array)->nelts == 0

#define sb_array_base(array, type)                                            \
    ((type*)((array)->elts))

#define sb_array_clear(array)                                                 \
    ((array)->nelts = 0) 

#define sb_array_size(array)                                                  \
    (array)->nelts

#define sb_array_capacity(array)                                              \
    ((array)->nalloc - (array)->nelts)

#define sb_array_pop(array)                                                   \
    do {                                                                      \
        if ((array)->nelts != 0) {                                            \
            (array)->nelts--;                                                 \
        }                                                                     \
    } while (false)

#define sb_array_pop_n(array, n)                                              \
    do {                                                                      \
        if (n > (array)->nelts) {                                             \
            (array)->nelts = 0;                                               \
        } else {                                                              \
            (array)->nelts -= n;                                              \
        }                                                                     \
    } while (false)

#define sb_array_for_each(array, base, index)                                 \
    for ((base) = (array)->elts, (index) = 0;                                 \
         (sb_size_t)(index) < (array)->nelts;                                 \
         (index)++)

sb_array_t *sb_array_create(sb_size_t size);
sb_array_t *sb_array_create_n(sb_size_t size, sb_size_t n);
sb_void_t sb_array_destroy(sb_array_t *array);
sb_array_t *sb_array_dup(sb_array_t *array);
sb_void_t sb_array_swap(sb_array_t *lhs, sb_array_t *rhs);
sb_void_t sb_array_shrink(sb_array_t *array);
sb_bool_t sb_array_reserve(sb_array_t *array, sb_size_t n);
sb_bool_t sb_array_resize(sb_array_t *array, sb_size_t n);
sb_void_t *sb_array_push(sb_array_t *array);
sb_void_t *sb_array_push_n(sb_array_t *array, sb_size_t n);
sb_bool_t sb_array_erase(sb_array_t *a, sb_uint_t index);
sb_bool_t sb_array_erase_n(sb_array_t *array, sb_uint_t index, sb_size_t n);
sb_bool_t sb_array_insert(sb_array_t *array,
    sb_uint_t index, sb_void_t *ptr);
sb_bool_t sb_array_insert_n(sb_array_t *array,
    sb_uint_t index, sb_void_t *ptr, sb_size_t n);
sb_void_t sb_array_range(sb_array_t *array,
    sb_ssize_t start, sb_ssize_t end);
sb_void_t sb_array_magic(sb_array_t *array, sb_array_magic_pt magic);

#endif
