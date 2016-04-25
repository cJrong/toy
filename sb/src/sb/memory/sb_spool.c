
#include <sb/sb_config.h>
#include <sb/memory/sb_spool.h>
#include <sb/memory/sb_alloc.h>
#include <sb/memory/sb_memops.h>


static
sb_void_t *__sb_spool_alloc_block__(sb_spool_t *spool, sb_size_t size);
static 
sb_void_t *__sb_spool_alloc_large__(sb_spool_t *spool, sb_size_t size);


sb_spool_t *sb_spool_create(sb_size_t alignment, sb_size_t size)
{
    sb_spool_t *spool;
    sb_uint_t  pagesize;

    spool = (sb_spool_t*)sb_align_alloc(alignment, size);
    if (!spool) {
        return nullptr;
    }

    spool->data.last   = (sb_bytep_t)spool + sizeof(sb_spool_t);
    spool->data.end    = (sb_bytep_t)spool + size;
    spool->data.next   = nullptr;
    spool->data.failed = 0;

    size             = size - sizeof(sb_spool_t);
    pagesize         = sb_get_page_size() - 1;
    spool->max       = (size < pagesize) ? size : pagesize;
    spool->alignment = alignment;

    spool->large   = nullptr;
    spool->current = spool;
    
    return spool;
}


sb_void_t sb_spool_destroy(sb_spool_t *spool)
{
    sb_spool_large_t *large;
    sb_spool_t       *current, *next;

    for (large = spool->large; large; large = large->next) {
        if (large->data) {
            sb_align_free(large->data);
        }
    }

    for (current = spool, next = spool->data.next; ;
         current = next, next = next->data.next) {
        sb_align_free(current);

        if (next == nullptr) {
            break;
        }
    }
}


sb_void_t sb_spool_reset(sb_spool_t *spool)
{
    sb_spool_large_t *large;
    sb_spool_t       *current;

    for (large = spool->large; large; large = large->next) {
        if (large->data) {
            sb_align_free(large);
        }
    }

    for (current = spool; current; current = current->data.next) {
        current->data.last   = (sb_bytep_t)spool + sizeof(sb_spool_t);
        current->data.failed = 0;
    }

    spool->current = spool;
    spool->large   = nullptr;
}


sb_void_t *sb_spool_alloc(sb_spool_t *smpool, sb_size_t size)
{
    if (size <= 0) {
        return nullptr;

    } else if (size <= smpool->max) {
        sb_spool_t *current = smpool->current;

        do {
            sb_byte_t *last = (sb_byte_t *) sb_align_ptr(current->data.last, SB_ALIGNMENT);

            if ((sb_size_t)(current->data.end - last) >= size) {
                current->data.last = last + size;

                return last;
            }

            current = current->data.next;

        } while (current);

        return __sb_spool_alloc_block__(smpool, size);
    }

    return __sb_spool_alloc_large__(smpool, size);
}


sb_void_t *sb_spool_nalloc(sb_spool_t *spool, sb_size_t size)
{
    if (size <= 0) {
        return nullptr;

    } else if (size <= spool->max) {
        sb_spool_t *current = spool->current;

        do {
            sb_byte_t *last = current->data.last;

            if ((sb_size_t)(current->data.end - last) >= size) {
                current->data.last = last + size;

                return last;
            }

            current = current->data.next;

        } while (current);

        return __sb_spool_alloc_block__(spool, size);
    }

    return __sb_spool_alloc_large__(spool, size);
}


sb_void_t *sb_spool_align_alloc(sb_spool_t *spool, sb_size_t alignment,
    sb_size_t size)
{
    sb_void_t        *mem;
    sb_spool_large_t *large;

    mem = sb_align_alloc(alignment, size);
    if (mem == nullptr) {
        return nullptr;
    }

    large = sb_spool_alloc(spool, sizeof(sb_spool_large_t));
    if (large == nullptr) {
        sb_align_free(large);
        return nullptr;
    }

    large->data = mem;
    large->next = spool->large;
    spool->large = large;

    return mem;
}


sb_bool_t sb_spool_free(sb_spool_t *pool, sb_void_t *ptr)
{
    sb_spool_large_t *large;

    for (large = pool->large; large; large = large->next) {
        if (large->data == ptr) {
            sb_align_free(large->data);
            large->data = nullptr;

            return true;
        }
    }

    return false;
}


static 
sb_void_t *__sb_spool_alloc_block__(sb_spool_t *spool, sb_size_t size)
{
    sb_spool_t *pool, *mount, *current;
    sb_byte_t  *mem;
    sb_size_t  psize;

    psize = (sb_size_t)(spool->data.end - (sb_bytep_t)spool);

    mem = sb_align_alloc(spool->alignment, psize);
    if (mem == nullptr) {
        return nullptr;
    }

    pool = (sb_spool_t*) mem;

    pool->data.end  = mem + psize;
    pool->data.next = nullptr;
    pool->data.failed = 0;

    mem += sizeof(sb_spool_data_t);
    mem = (sb_byte_t *)sb_align_ptr(mem, SB_ALIGNMENT);
    pool->data.last = mem + size;

    current = spool->current;
    for (mount = current; mount->data.next; mount = mount->data.next) {
        if (mount->data.failed++ > 4) {
            current = mount->data.next;
        }
    }

    mount->data.next = pool;
    spool->current = current ? current : pool;

    return mem;
}


static
sb_void_t *__sb_spool_alloc_large__(sb_spool_t *spool, sb_size_t size)
{
    sb_void_t        *mem;
    sb_spool_large_t *large;
    sb_uint_t        err;

    mem = sb_align_alloc(spool->alignment, size);
    if (mem == nullptr) {
        return nullptr;
    }

    for (err = 0, large = spool->large; large; large = large->next) {
        if (large->data == nullptr) {
            large->data = mem;
            return mem;
        }

        if (err++ > 3) {
            break;
        }
    }

    large = (sb_spool_large_t *)sb_spool_alloc(spool, sizeof(sb_spool_large_t));
    if (large == nullptr) {
        sb_align_free(mem);
        return nullptr;
    }

    large->data  = mem;
    large->next  = spool->large;
    spool->large = large;

    return mem;
}
