
#include <sb/sb_config.h>
#include <sb/memory/sb_pool.h>
#include <sb/memory/sb_alloc.h>


static sb_inline
sb_size_t  __sb_pool_round_up__(sb_pool_t *pool, sb_size_t bytes);
static sb_inline
sb_size_t  __sb_pool_freelist_index__(sb_pool_t *pool, sb_size_t bytes);
static sb_inline
sb_bool_t  __sb_pool_cleanup_chunk__(sb_rbtree_node_t *node);
static
sb_void_t *__sb_pool_refill__(sb_pool_t *pool, sb_size_t size);
static
sb_void_t *__sb_pool_chunk_allocate__(sb_pool_t *pool, sb_size_t size,
    sb_size_t *nobjs);
static
sb_void_t  __sb_rbtree_insert_chunk__(sb_rbtree_t *tree,
    sb_rbtree_node_t *node);
static
sb_void_t  __sb_rbtree_remove_chunk__(sb_rbtree_t *tree,
    sb_rbtree_node_t *node);


sb_pool_t *sb_pool_create(sb_uint_t nobjs, sb_uint_t alignment,
    sb_uint_t max_size)
{
    sb_size_t nfreelist, poolsize, i;
    sb_pool_t *pool;
    
    nfreelist = max_size / alignment;
    poolsize  = sizeof(sb_pool_t) + sizeof(sb_pool_object_t *) * nfreelist;

    pool = (sb_pool_t *) sb_align_alloc(alignment, poolsize);
    if (!pool) {
        return nullptr;
    }

    pool->free_lists  = (sb_pool_object_t **) &pool[1];
    pool->start_free = nullptr;
    pool->end_free   = nullptr;
    pool->heap_size  = 0;
    pool->alignment  = alignment;
    pool->max_size   = max_size;
    pool->nobjs      = nobjs;

    for (i = 0; i < nfreelist; i++) {
        pool->free_lists[i] = nullptr;
    }

    sb_rbtree_init(&pool->rbtree);

    return pool;
}


sb_void_t sb_pool_destroy(sb_pool_t *pool)
{
    sb_rbtree_clear(&pool->rbtree, __sb_pool_cleanup_chunk__);
    sb_align_free(pool);
}


sb_void_t *sb_pool_alloc(sb_pool_t *pool, sb_size_t size)
{
    sb_pool_object_t **freelist, *result;
    sb_rbtree_node_t *chunk;

    if (size > pool->max_size) {
        chunk = sb_align_alloc(pool->alignment, sizeof(sb_rbtree_node_t) + size);
        __sb_rbtree_insert_chunk__(&pool->rbtree, chunk);
        return ++chunk;

    } else if (size <= 0) {
        return nullptr;
    }

    freelist = pool->free_lists + __sb_pool_freelist_index__(pool, size);
    result = *freelist;

    if (result == nullptr) {
        return __sb_pool_refill__(pool, __sb_pool_round_up__(pool, size));
    }

    *freelist = result->freelist;
    return result;
}


sb_void_t sb_pool_free(sb_pool_t *pool, sb_void_t *ptr, sb_size_t size)
{
    sb_pool_object_t **freelist;
    sb_pool_object_t *recycle = (sb_pool_object_t*) ptr;

    if (size > pool->max_size) {
        sb_rbtree_node_t* large = (sb_rbtree_node_t*) ptr;        
        __sb_rbtree_remove_chunk__(&pool->rbtree, --large);
        sb_align_free(large);
        return ;
    }

    freelist = pool->free_lists + __sb_pool_freelist_index__(pool, size);
    recycle->freelist = *freelist;
    *freelist = recycle->freelist;
}


static
sb_void_t *__sb_pool_refill__(sb_pool_t *pool, sb_size_t size)
{
    sb_pool_object_t **freelist, *result, *next, *current;
    sb_void_t        *chunk;
    sb_size_t        nobjs = pool->nobjs;
    sb_size_t        i;

    chunk = __sb_pool_chunk_allocate__(pool, size, &nobjs);

    if (nobjs == 1 || chunk == nullptr) {
        return chunk;
    }

    freelist  = pool->free_lists + __sb_pool_freelist_index__(pool, size);
    result    = (sb_pool_object_t*) chunk;
    *freelist = next = (sb_pool_object_t*)((sb_bytep_t)chunk + size);

    for (i = 2; ; i++) {
        current = next;
        next    = (sb_pool_object_t*)((sb_bytep_t)next + size);

        if (nobjs == i) {
            current->freelist = nullptr;
            break;
        }

        current->freelist = next;
    }

    return result;
}


static
sb_void_t *__sb_pool_chunk_allocate__(sb_pool_t *pool, sb_size_t size, 
    sb_size_t *nobjs)
{
    sb_size_t residue = pool->end_free - pool->start_free;
    sb_size_t total = size * (*nobjs);
    sb_void_t *result;

    if (residue >= total) {
        result = pool->start_free;
        pool->start_free += total;
        return result;

    } else if (residue >= size) {
        *nobjs = residue / size;
        total = size * (*nobjs);
        result = pool->start_free;
        pool->start_free += total;
        return result;

    } else {
        sb_pool_object_t **freelist;
        sb_rbtree_node_t *chunk;
        sb_size_t        bytes_to_get;

        bytes_to_get = 2 * total + 
            __sb_pool_round_up__(pool, pool->heap_size >> 4);

        if (residue > 0) {
            sb_pool_object_t *left_over;
            
            freelist  = pool->free_lists +
                    __sb_pool_freelist_index__(pool, residue);

            left_over = (sb_pool_object_t *)pool->start_free;
            left_over->freelist = *freelist;

            *freelist = left_over;
        }

        chunk = (sb_rbtree_node_t *) sb_align_alloc(pool->alignment,
            sizeof(sb_rbtree_node_t) + bytes_to_get);

        if (chunk == nullptr) {
            sb_size_t i;

            for (i = size; i <= pool->max_size; i += pool->alignment) {
                sb_pool_object_t  *exploit;

                freelist = pool->free_lists + __sb_pool_freelist_index__(pool, i);
                exploit  = *freelist;

                if (exploit != nullptr) {
                    *freelist = exploit->freelist;

                    pool->start_free = (sb_bytep_t)exploit;
                    pool->end_free   = pool->start_free + i;

                    return __sb_pool_chunk_allocate__(pool, size, nobjs);
                }
            }

            return nullptr;
        }

        pool->start_free = (sb_bytep_t) chunk + sizeof(sb_rbtree_node_t);
        pool->end_free   = pool->start_free + bytes_to_get;
        pool->heap_size += bytes_to_get;

        __sb_rbtree_insert_chunk__(&pool->rbtree, chunk);

        return __sb_pool_chunk_allocate__(pool, size, nobjs);
    }
}


static sb_inline
sb_size_t __sb_pool_round_up__(sb_pool_t *pool, sb_size_t bytes) {
    return (bytes + pool->alignment - 1) & ~(pool->alignment - 1);
}


static sb_inline
sb_size_t __sb_pool_freelist_index__(sb_pool_t *pool, sb_size_t bytes) {
    return (bytes + pool->alignment - 1) / pool->alignment - 1;
}


static
sb_void_t __sb_rbtree_insert_chunk__(sb_rbtree_t *tree, sb_rbtree_node_t *node)
{
    sb_rbtree_node_t **current = &tree->root, *parent = nullptr;

    while (*current) {
        parent = *current;

        if (node == parent) {
            return;
        }

        current = node < parent ? &parent->left : &parent->right;
    }

    sb_rbtree_link_node(current, parent, node);
    sb_rbtree_insert(tree, node);
}


static
sb_void_t __sb_rbtree_remove_chunk__(sb_rbtree_t *tree, sb_rbtree_node_t *node)
{
    sb_rbtree_node_t *current = tree->root;

    while (current) {
        if (current == node) {
            break;
        }

        current = current < node ? current->left : current->right;
    }

    if (current) {
        sb_rbtree_remove(tree, current);
    }
}


static sb_inline
sb_bool_t __sb_pool_cleanup_chunk__(sb_rbtree_node_t *node)
{
    sb_align_free(node);
    return true;
}
