

#include <sb/sb_config.h>
#include <sb/memory/sb_alloc.h>
#include <sb/memory/sb_memops.h>
#include <sb/container/sb_array.h>

static sb_inline
sb_bool_t __sb_array_resize__(sb_array_t *array, sb_size_t n);


sb_array_t* sb_array_create(sb_size_t size)
{
    sb_array_t *array;

    array = (sb_array_t*) sb_alloc(sizeof(sb_array_t));
    if (!array) {
        return nullptr;
    }
 
    array->elts   = nullptr;
    array->nelts  = 0;
    array->nalloc = 0;
    array->size   = size;

    return array;
}


sb_array_t *sb_array_create_n(sb_size_t size, sb_size_t n)
{
    sb_array_t *array;

    array = (sb_array_t*) sb_alloc(sizeof(sb_array_t));
    if (!array) {
        return nullptr;
    }

    array->elts = (sb_array_t*) sb_alloc(size * n);
    if (!array->elts) {
        return nullptr;
    }

    array->nelts  = 0;
    array->nalloc = n;
    array->size   = size;

    return array;
}


sb_void_t sb_array_destroy(sb_array_t *array)
{
    if (array->elts) {
        sb_free(array->elts);
    }
    sb_free(array);
}


sb_array_t *sb_array_dup(sb_array_t *array)
{
    sb_array_t *dst = sb_array_create_n(array->size, array->nelts);
    if (!dst) {
        return nullptr;
    }

    sb_memcopy(sb_array_push_n(dst, array->nelts),
        array->elts, array->nelts * array->size);

    return dst;
}


sb_void_t sb_array_swap(sb_array_t *lhs, sb_array_t *rhs)
{
    sb_array_t tmp;
    tmp = *lhs;
    *lhs = *rhs;
    *rhs = tmp;
}


sb_void_t sb_array_shrink(sb_array_t *array)
{
    __sb_array_resize__(array, array->nelts);
}


sb_bool_t sb_array_reserve(sb_array_t *array, sb_size_t n)
{
    sb_size_t nfree = array->nalloc - array->nelts;
    if (nfree >= n) {
        return true;
    }

    if (!__sb_array_resize__(array, array->nelts + (n - nfree))) {
        return false;
    }

    return true;
}


sb_bool_t sb_array_resize(sb_array_t *array, sb_size_t n)
{
    sb_void_t *elt;
    sb_size_t  nalloc;

    if (array->nelts >= n) {
        array->nelts = n;
        return true;
    }
    
    nalloc = n - array->nelts;
    elt    = sb_array_push_n(array, nalloc);
    if (!elt) {
        return false;
    }

    sb_memzero(elt, nalloc * array->size);
    return true;
}


sb_void_t *sb_array_push(sb_array_t *array)
{
    sb_void_t *elt;

    if (array->nelts == array->nalloc) {
        sb_size_t n;

        if (array->nalloc == 0) {
            n = 2;

        } else {
            n = array->nalloc * 2;
        }

        if (!__sb_array_resize__(array, n)) {
            return nullptr;
        }
    }

    elt = (sb_bytep_t)array->elts + array->size * array->nelts;
    array->nelts++;

    return elt;
}


sb_void_t *sb_array_push_n(sb_array_t *array, sb_size_t n) 
{
    sb_void_t *elt;

    if (array->nelts + n > array->nalloc) {
        sb_size_t nalloc = 2 * (n >= array->nalloc ? n : array->nalloc);
        if (!__sb_array_resize__(array, nalloc)) {
            return nullptr;
        }
    }
    
    elt = (sb_bytep_t)array->elts + array->size * array->nelts;
    array->nelts += n;

    return elt;
}


sb_bool_t sb_array_erase(sb_array_t *array, sb_uint_t index)
{
    sb_byte_t *p;
    sb_uint_t  move_size;

    if (index >= array->nelts) {
        return false;
    }

    p = (sb_bytep_t)array->elts;
    move_size = array->size * (array->nelts - (index + 1));

    if (move_size) {
        sb_memcopy(p + array->size * index,
            p + array->size * (index + 1), move_size);
    }

    array->nelts--;
    return true;
}


sb_bool_t sb_array_erase_n(sb_array_t *array, sb_uint_t index, sb_size_t n)
{
    sb_byte_t *p;
    sb_uint_t  move_size;

    if ((index >= array->nelts) || (index + n > array->nelts)) {
        return false;
    }

    p = (sb_bytep_t)array->elts;
    move_size = array->size * (array->nelts - index - n);

    if (move_size && n) {
        sb_memcopy(p + array->size * index,
            p + array->size * (index + n), move_size);
    }

    array->nelts -= n;
    return true;
}


sb_void_t sb_array_range(sb_array_t *array, sb_ssize_t start, sb_ssize_t end)
{
    sb_size_t newlen;

    if (array->nelts == 0) {
        return ;
    }

    start = start >= 0 ? start : 0;
    end = end >= 0 ? end : array->nelts - 1;
    newlen = (sb_size_t)(start > end ? 0 : end - start + 1);
    newlen = array->nelts > newlen ? newlen : array->nelts;

    if (start && newlen) {
        sb_memmove(array->elts,
            (sb_bytep_t)array->elts + start * array->size,
            newlen * array->size);
    }

    array->nelts = newlen;
}


sb_void_t sb_array_magic(sb_array_t *array, sb_array_magic_pt magic)
{
    sb_byte_t *base;
    sb_int_t   index;

    sb_array_for_each(array, base, index) {
        if (!magic((sb_void_t*)(base + (index * array->size)))) {
            return ;
        }
    }
}


sb_bool_t sb_array_insert(sb_array_t *array,
    sb_uint_t index, sb_void_t *ptr)
{
    sb_byte_t *p;
    sb_uint_t  move_size;

    if (index > array->nelts) {
        return false;

    } else if (array->nalloc == array->nelts) {
        if (!sb_array_push(array)) {
            return false;
        }

    } else {
        array->nelts++;
    }

    p = (sb_bytep_t)array->elts;
    move_size = array->size * (array->nelts - index);

    if (move_size) {
        sb_memmove(p + array->size * (index + 1), p + array->size * index, move_size);
    }

    sb_memcopy(p + array->size * index, ptr, array->size);

    return true;
}


sb_bool_t sb_array_insert_n(sb_array_t *array,
    sb_uint_t index, sb_void_t *ptr, sb_size_t n)
{

    sb_byte_t *p;
    sb_uint_t  move_size;

    if (index > array->nelts) {
        return false;

    } else if (array->nalloc + n > array->nelts) {
        if (!sb_array_push_n(array, n)) {
            return false;
        }

    } else {
        array->nelts += n;
    }

    p = (sb_bytep_t)array->elts;
    move_size = array->size * (array->nelts - (index + n));

    if (move_size) {
        sb_memmove(p + array->size * (index + n), p + array->size * index, move_size);
    }

    sb_memcopy(p + array->size * index, ptr, array->size * n);

    return true;
}


static sb_inline
sb_bool_t __sb_array_resize__(sb_array_t *array, sb_size_t n)
{
    sb_size_t size = array->size * n;
 
    if (size) {
        array->elts = sb_ralloc(array->elts, size);
        if (!array->elts) {
            return nullptr;
        }
    }

    array->nalloc = n;
    
    return true;
}
