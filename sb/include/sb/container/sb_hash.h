

#ifndef __SB__HASH__H__INCLUDE__
#define __SB__HASH__H__INCLUDE__

#include <sb/sb_config.h>
#include <sb/container/sb_hlist.h>

typedef struct sb_hash_ops_s {
    sb_uint_t (*hashfn)(const sb_hlist_node_t *node);
    sb_int_t  (*compare)(const sb_hlist_node_t *lhs, 
        const sb_hlist_node_t *rhs);
    sb_hlist_node_t *(*dup)(const sb_hlist_node_t *node);
    sb_void_t (*destructor)(sb_hlist_node_t *node);
}sb_hash_ops_t;

typedef struct sb_hash_table_s {
    sb_hlist_t  *bucket;
    sb_ulong_t   size;
    sb_ulong_t   used;
}sb_hash_table_t;

typedef struct sb_hash_s {
    sb_hash_table_t ht[2];
    sb_hash_ops_t  *ops;
    sb_int_t        rehashidx;
    sb_int_t        iterators;
    sb_size_t       bucket_init_size;
}sb_hash_t;

typedef struct sb_hash_iterator_s {
    sb_hash_t       *hash;
    sb_hlist_node_t *hn;
    sb_hlist_node_t *nexthn;
    sb_int_t         table;
    sb_long_t        index;
    sb_bool_t        safe;
}sb_hash_iterator_t;

typedef sb_bool_t(*sb_hash_magic_pt)(sb_hlist_node_t*);

static sb_inline
sb_hash_iterator_t sb_hash_iterator_create(sb_hash_t *hash, sb_bool_t safe)
{
    sb_hash_iterator_t iterator;
    iterator.hash   = hash;
    iterator.hn     = nullptr;
    iterator.nexthn = nullptr;
    iterator.table  = 0;
    iterator.index  = -1;
    iterator.safe   = safe;
    return iterator;
}

#define sb_hash_iterator_destroy(iterator)                                    \
    do {                                                                      \
        if ((iterator).safe) {                                                \
            (iterator).hash->iterators--;                                     \
        }                                                                     \
        (iterator).hash = nullptr;                                            \
    } while (false)

#define sb_hash_iterator_equals(lhs, rhs)                                     \
    (((lhs).hash   == (rhs).hash) &&                                          \
     ((lhs).hn     == (rhs).hn) &&                                            \
     ((lhs).nexthn == (rhs).nexthn) &&                                        \
     ((lhs).table  == (rhs).table) &&                                         \
     ((lhs).index  == (rhs).index))

#define sb_hash_element(ptr, type, member)                                    \
    sb_hlist_element(ptr, type, member)

#define sb_hash_iterator_element(iter, type, member)                          \
    (sb_hash_iterator_equals((iter), sb_hash_iterator_create(hash, false)) ?  \
        nullptr : sb_hlist_element((iter).hn, type, member))

#define sb_hash_size(hash)                                                    \
    ((hash)->ht[0].used + (hash)->ht[1].used)

#define sb_hash_slots(hash)                                                   \
    ((hash)->ht[0].size + (hash)->ht[1].size)

#define sb_hash_is_rehashing(hash)                                            \
    ((hash)->rehashidx != -1)

#define sb_hash_is_empty(hash)                                                \
    (sb_hash_size(hash) == 0)

#define sb_hash_for_each(hash, iterator)                                      \
    for (sb_hash_iterator_equals((iterator),                                  \
                sb_hash_iterator_create(hash, false)) ?                       \
            sb_hash_iterator_next(&(iterator)) : 0;                           \
         !sb_hash_iterator_equals(iterator,                                   \
            sb_hash_iterator_create(hash, false));                            \
         sb_hash_iterator_next(&(iterator)))

sb_hash_t *sb_hash_create(sb_hash_ops_t *ops, sb_size_t bucket_init_size);
sb_void_t sb_hash_destroy(sb_hash_t *hash);
sb_void_t sb_hash_clear(sb_hash_t *hash);
sb_bool_t sb_hash_expand(sb_hash_t *hash, sb_size_t size);
sb_bool_t sb_hash_shrink(sb_hash_t *hash);
sb_bool_t sb_hash_rehash(sb_hash_t *hash, sb_size_t step);
sb_bool_t sb_hash_insert(sb_hash_t *hash, sb_hlist_node_t *node);
sb_bool_t sb_hash_update(sb_hash_t *hash, sb_hlist_node_t *node);
sb_hlist_node_t *sb_hash_search(sb_hash_t *hash, sb_hlist_node_t *node);
sb_bool_t sb_hash_remove(sb_hash_t *hash, sb_hlist_node_t *node);
sb_void_t sb_hash_magic(sb_hash_t *hash, sb_hash_magic_pt magic);

sb_bool_t sb_hash_iterator_next(sb_hash_iterator_t *iterator);

#endif
