

#ifndef __SB__SPOOL__H__INCLUDE__
#define __SB__SPOOL__H__INCLUDE__

#include <sb/sb_config.h>

typedef struct sb_spool_large_s    sb_spool_large_t;
typedef struct sb_spool_data_s     sb_spool_data_t;
typedef struct sb_spool_s          sb_spool_t;

enum {
    SB_SPOOL_ALIGNMENT = 4,
    SB_SPOOL_CHUNKSIZE = 4096
};

struct sb_spool_large_s {
    sb_spool_large_t   *next;
    sb_bytep_t         data;
};

struct sb_spool_data_s {
    sb_uint_t     failed;
    sb_byte_t     *last;
    sb_byte_t     *end;
    sb_spool_t    *next;
};

struct sb_spool_s {
    sb_spool_data_t    data;
    sb_size_t          max;
    sb_size_t          alignment;
    sb_spool_large_t   *large;
    sb_spool_t         *current;
};

sb_spool_t *sb_spool_create(sb_size_t alignment, sb_size_t size);
sb_void_t sb_spool_destroy(sb_spool_t *spool);
sb_void_t sb_spool_reset(sb_spool_t *spool);
sb_void_t *sb_spool_alloc(sb_spool_t *spool, sb_size_t size);
sb_void_t *sb_spool_nalloc(sb_spool_t *spool, sb_size_t size);
sb_void_t *sb_spool_align_alloc(sb_spool_t *spool,
    sb_size_t alignment, sb_size_t size);
sb_bool_t sb_spool_free(sb_spool_t *pool, sb_void_t *ptr);

#define sb_spool_default()                                                    \
    sb_spool_create(SB_SPOOL_ALIGNMENT, SB_SPOOL_CHUNKSIZE)

#endif
