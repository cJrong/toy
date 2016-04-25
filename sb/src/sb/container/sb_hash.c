

#include <sb/sb_config.h>
#include <sb/memory/sb_alloc.h>
#include <sb/memory/sb_memops.h>
#include <sb/container/sb_hash.h>


#ifndef SB_HASH_BUCKET_MAX_EXPAND_LIMIT
#define SB_HASH_BUCKET_MAX_EXPAND_LIMIT     (1024*1024)
#endif


static sb_inline 
sb_void_t __sb_hash_init__(sb_hash_t *hash, 
    sb_hash_ops_t *ops, sb_size_t bucket_init_size);
static sb_inline 
sb_int_t  __sb_hash_index__(sb_hash_t *hash, sb_hlist_node_t *node);
static
sb_bool_t __sb_hash_rehash__(sb_hash_t *hash, sb_size_t step);
static
sb_bool_t __sb_hash_resize__(sb_hash_t *hash, sb_size_t size);
static sb_inline
sb_void_t __sb_hash_table_reset__(sb_hash_table_t *ht);
static sb_inline 
sb_size_t __sb_hash_table_calc_bucket_size__(sb_hash_t *hash);
static sb_inline
sb_bool_t __sb_hash_table_if_needed_expand__(sb_hash_t *hash);
static
sb_void_t __sb_hash_table_clear__(sb_hash_t *hash, sb_hash_table_t *ht);


sb_hash_t *sb_hash_create(sb_hash_ops_t *ops, sb_size_t bucket_init_size)
{
    sb_hash_t *hash;
    
    hash = (sb_hash_t*)sb_alloc(sizeof(sb_hash_t));
    if (!hash) {
        return nullptr;
    }

    __sb_hash_init__(hash, ops, bucket_init_size);

    return hash;
}


sb_void_t sb_hash_destroy(sb_hash_t *hash)
{
    __sb_hash_table_clear__(hash, &hash->ht[0]);
    __sb_hash_table_clear__(hash, &hash->ht[1]);

    sb_free(hash);
}


sb_void_t sb_hash_clear(sb_hash_t *hash)
{
    __sb_hash_table_clear__(hash, &hash->ht[0]);
    __sb_hash_table_clear__(hash, &hash->ht[1]);

    hash->rehashidx = -1;
    hash->iterators = 0;
}


sb_bool_t sb_hash_expand(sb_hash_t *hash, sb_size_t size)
{
    sb_size_t realsize;

    if (sb_hash_is_rehashing(hash) || hash->ht[0].used > size) {
        return false;
    }

    realsize = __sb_hash_table_calc_bucket_size__(hash);
    realsize = realsize > size ? realsize : size;

    return __sb_hash_resize__(hash, realsize);
}


sb_bool_t sb_hash_shrink(sb_hash_t *hash)
{
    sb_size_t minimal;

    if (sb_hash_is_rehashing(hash)) {
        return false;
    }

    minimal = hash->ht[0].used;
    if (minimal < hash->bucket_init_size) {
        minimal = hash->bucket_init_size;
    }

    return __sb_hash_resize__(hash, minimal);
}


sb_bool_t sb_hash_rehash(sb_hash_t *hash, sb_size_t step)
{
    if (!hash->iterators) {
        return __sb_hash_rehash__(hash, step);
    }

    return false;
}


sb_bool_t sb_hash_insert(sb_hash_t *hash, sb_hlist_node_t *node)
{
    sb_hlist_node_t *newnode;
    sb_hash_table_t *ht;
    sb_int_t         index;

    if (sb_hash_is_rehashing(hash)) {
        sb_hash_rehash(hash, 1);
    }

    if ((index = __sb_hash_index__(hash, node)) == -1) {
        return false;
    }

    newnode = hash->ops->dup ? hash->ops->dup(node) : node;
    if (!newnode) {
        return false;
    }

    ht = sb_hash_is_rehashing(hash) ? &hash->ht[1] : &hash->ht[0];
    
    sb_hlist_insert(&ht->bucket[index], newnode);
    ht->used++;

    return true;
}


sb_bool_t sb_hash_update(sb_hash_t *hash, sb_hlist_node_t *node)
{
    sb_hlist_node_t *newnode;
    sb_hash_table_t *ht;
    sb_hlist_t       head;
    sb_uint_t        hash_key;
    sb_int_t         i;
    sb_int_t         index;

    if (sb_hash_is_rehashing(hash)) {
        sb_hash_rehash(hash, 1);
    }

    if (!__sb_hash_table_if_needed_expand__(hash)) {
        return false;
    }

    newnode = hash->ops->dup ? hash->ops->dup(node) : node;
    if (!newnode) {
        return false;
    }

    hash_key = hash->ops->hashfn(newnode);

    for (i = 0; i < 2; i++) {
        sb_hlist_node_t *hn;

        index = hash_key % hash->ht[i].size;
        head = hash->ht[i].bucket[index];
        sb_hlist_for_each(&head, hn) {
            if (hash->ops->compare(hn, newnode) == 0) {
                sb_hlist_replace(hn, newnode);

                if (hash->ops->destructor) {
                    hash->ops->destructor(hn);
                }
                return true;
            }
        }

        if (!sb_hash_is_rehashing(hash)) {
            break;
        }
    }

    ht = sb_hash_is_rehashing(hash) ? &hash->ht[1] : &hash->ht[0];
    sb_hlist_insert(&ht->bucket[index], newnode);
    ht->used++;
    return true;
}


sb_hlist_node_t *sb_hash_search(sb_hash_t *hash, sb_hlist_node_t *node)
{
    sb_uint_t hash_key;
    sb_int_t  i;

    if (hash->ht[0].size == 0) {
        return nullptr;
    }

    if (sb_hash_is_rehashing(hash)) {
        sb_hash_rehash(hash, 1);
    }

    hash_key = hash->ops->hashfn(node);

    for (i = 0; i < 2; i++) {
        sb_hlist_node_t *hn;
        sb_int_t index = hash_key % hash->ht[i].size;

        sb_hlist_for_each(&hash->ht[i].bucket[index], hn) {
            if (hash->ops->compare(hn, node) == 0) {
                return hn;
            }
        }

        if (!sb_hash_is_rehashing(hash)) {
            break;
        }
    }

    return nullptr;
}


sb_bool_t sb_hash_remove(sb_hash_t *hash, sb_hlist_node_t *node)
{
    sb_uint_t hash_key;
    sb_int_t  i;

    if (hash->ht[0].size == 0) {
        return false;
    }

    if (sb_hash_is_rehashing(hash)) {
        sb_hash_rehash(hash, 1);
    }

    hash_key = hash->ops->hashfn(node);

    for (i = 0; i < 2; i++) {
        sb_hlist_node_t *hn;
        sb_int_t index = hash_key % hash->ht[i].size;

        sb_hlist_for_each(&hash->ht[i].bucket[index], hn) {
            if (hash->ops->compare(hn, node) == 0) {
                sb_hlist_remove(hn);

                if (hash->ops->destructor) {
                    hash->ops->destructor(hn);
                }
                return true;
            }
        }

        if (!sb_hash_is_rehashing(hash)) {
            break;
        }
    }

    return false;
}


sb_void_t sb_hash_magic(sb_hash_t *hash, sb_hash_magic_pt magic)
{
    sb_hash_iterator_t begin = sb_hash_iterator_create(hash, true);
    sb_hash_iterator_t end = sb_hash_iterator_create(hash, true);

    for (sb_hash_iterator_next(&begin); 
         !sb_hash_iterator_equals(begin, end);
         sb_hash_iterator_next(&begin)) {
        if (!magic(begin.hn)) {
            break;
        }
    }

    sb_hash_iterator_destroy(begin);
    sb_hash_iterator_destroy(end);
}


sb_bool_t sb_hash_iterator_next(sb_hash_iterator_t *iterator)
{
    while (true) {
        if (!iterator->hn) {
            sb_hash_table_t *ht = &iterator->hash->ht[iterator->table];
            
            if (iterator->index == -1 && iterator->table == 0) {
                if (iterator->safe) {
                    iterator->hash->iterators++;
                }
            }

            iterator->index++;

            if (iterator->index >= (sb_long_t) ht->size) {
                if (sb_hash_is_rehashing(iterator->hash)) {
                    iterator->index = 0;
                    ht = &iterator->hash->ht[++iterator->table];

                } else {
                    break;
                }
            }

            iterator->hn = ht->bucket[iterator->index].first;

        } else {
            iterator->hn = iterator->nexthn;
        }

        if (iterator->hn) {
            iterator->nexthn = iterator->hn->next;
            return true;
        }
    }

    /* end iterator */
    iterator->table  = 0;
    iterator->index  = -1;

    return false;
}


static sb_inline
sb_void_t __sb_hash_init__(sb_hash_t *hash,
    sb_hash_ops_t *ops, sb_size_t bucket_init_size)
{
    __sb_hash_table_reset__(&hash->ht[0]);
    __sb_hash_table_reset__(&hash->ht[1]);

    hash->ops = ops;

    hash->rehashidx = -1;

    hash->iterators = 0;

    hash->bucket_init_size = bucket_init_size;
}


static sb_inline
sb_int_t  __sb_hash_index__(sb_hash_t *hash, sb_hlist_node_t *node)
{
    sb_uint_t hash_key;
    sb_int_t  i;
    sb_int_t  index;

    if (!__sb_hash_table_if_needed_expand__(hash)) {
        return -1;
    }

    hash_key = hash->ops->hashfn(node);

    for (i = 0; i < 2; i++) {
        sb_hlist_node_t *hn;

        index = hash_key % hash->ht[i].size;
        sb_hlist_for_each(&hash->ht[i].bucket[index], hn) {
            if (hash->ops->compare(hn, node) == 0) {
                return -1;
            }
        }

        if (!sb_hash_is_rehashing(hash)) {
            break;
        }
    }

    return index;
}


static
sb_bool_t __sb_hash_rehash__(sb_hash_t *hash, sb_size_t step)
{
    sb_int_t empty_visits = step * 10;

    if (!sb_hash_is_rehashing(hash)) {
        return false;
    }

    while (step-- && hash->ht[0].used != 0) {
        sb_hlist_node_t *hn, *nexthn;

        while (!hash->ht[0].bucket[hash->rehashidx].first) {
            hash->rehashidx++;
            if (--empty_visits) {
                return true;
            }
        }

        sb_hlist_safe_for_each(&hash->ht[0].bucket[hash->rehashidx], hn, nexthn) {
            sb_uint_t index = hash->ops->hashfn(hn) % hash->ht[1].size;
            sb_hlist_insert(&hash->ht[1].bucket[index], hn);
            hash->ht[0].used--;
            hash->ht[1].used++;
        }

        hash->ht[0].bucket[hash->rehashidx].first = nullptr;
        hash->rehashidx++;
    }

    if (hash->ht[0].used == 0) {
        sb_free(hash->ht[0].bucket);
        hash->ht[0] = hash->ht[1];
        __sb_hash_table_reset__(&hash->ht[1]);
        hash->rehashidx = -1;
        return false;
    }

    return true;
}


static
sb_bool_t __sb_hash_resize__(sb_hash_t *hash, sb_size_t size)
{
    sb_hash_table_t ht;
    sb_size_t       i;

    ht.bucket = (sb_hlist_t *)sb_alloc(sizeof(sb_hlist_t) * size);
    if (!ht.bucket) {
        return false;
    }

    for (i = 0; i < size; i++) {
        sb_hlist_init(&ht.bucket[i]);
    }

    ht.size = size;
    ht.used = 0;

    if (!hash->ht[0].bucket) {
        hash->ht[0] = ht;
        return true;
    }

    /* rehashing... */

    hash->ht[1] = ht;
    hash->rehashidx = 0;
    return true;
}


static sb_inline
sb_void_t __sb_hash_table_reset__(sb_hash_table_t *ht)
{
    ht->bucket = nullptr;
    ht->size  = 0;
    ht->used  = 0;
}


static sb_inline
sb_size_t __sb_hash_table_calc_bucket_size__(sb_hash_t *hash)
{
    sb_size_t retval = hash->ht[0].size;

    if (retval == 0) {
        return hash->bucket_init_size;
    }
    
    if (retval <= SB_HASH_BUCKET_MAX_EXPAND_LIMIT) {
        retval *= 2;

    } else {
        retval += SB_HASH_BUCKET_MAX_EXPAND_LIMIT;
    }

    return retval;
}


static sb_inline
sb_bool_t __sb_hash_table_if_needed_expand__(sb_hash_t *hash)
{
    if (sb_hash_is_rehashing(hash)) {
        return true;
    }

    if (hash->ht[0].size == 0) {
        sb_hash_expand(hash, hash->bucket_init_size);
    }
    
    if (hash->ht[0].used >= hash->ht[0].size && 
        hash->ht[0].used / hash->ht[0].size >= 5) {
        return sb_hash_expand(hash, hash->ht[0].used * 2);
    }
        
    return true;
}


static
sb_void_t __sb_hash_table_clear__(sb_hash_t *hash, sb_hash_table_t *ht)
{
    sb_ulong_t i;

    if (!ht->bucket) {
        return ;
    }

    for (i = 0; i < ht->size && ht->used > 0; i++) {
        sb_hlist_t head;
        sb_hlist_node_t *hn, *nexthn;

        if ((head = ht->bucket[i]).first == nullptr) {
            continue;
        }

        sb_hlist_safe_for_each(&head, hn, nexthn) {
            if (hash->ops->destructor) {
                hash->ops->destructor(hn);
            }
            ht->used--;
        }
    }

    sb_free(ht->bucket);

    __sb_hash_table_reset__(ht);
}
