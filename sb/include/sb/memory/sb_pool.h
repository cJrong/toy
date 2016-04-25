

#ifndef __SB__POOL__H__INCLUDE__
#define __SB__POOL__H__INCLUDE__

#include <sb/sb_config.h>
#include <sb/container/sb_rbtree.h>

enum {
    SB_POOL_ALIGNMENT = 8,
    SB_POOL_NOBJECTS  = 20,
    SB_POOL_MAXBYTES  = 128
};

typedef union sb_pool_object_u {
    union sb_pool_object_u *freelist;
    sb_byte_t               freedata[1];
}sb_pool_object_t;

typedef struct sb_pool_s {
    sb_pool_object_t **free_lists;

    sb_byte_t        *start_free;
    sb_byte_t        *end_free;

    sb_uint_t        heap_size;

    sb_uint_t        alignment;
    sb_uint_t        max_size;
    sb_uint_t        nobjs;

    sb_rbtree_t      rbtree;
}sb_pool_t;

sb_pool_t *sb_pool_create(sb_uint_t nobjs, sb_uint_t alignment, sb_uint_t max_size);
sb_void_t sb_pool_destroy(sb_pool_t *pool);
sb_void_t *sb_pool_alloc(sb_pool_t *pool, sb_size_t size);
sb_void_t sb_pool_free(sb_pool_t *pool, sb_void_t *ptr, sb_size_t size);

#define sb_pool_default()                                                     \
    sb_pool_create(SB_POOL_NOBJECTS, SB_POOL_ALIGNMENT, SB_POOL_MAXBYTES)

#endif
