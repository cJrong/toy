

#include <sb/sb_config.h>
#include <sb/container/sb_bytes.h>
#include <sb/memory/sb_alloc.h>
#include <sb/memory/sb_memops.h>


static
sb_bool_t __sb_bytes_expand__(sb_bytes_t *bytes, sb_size_t size);


sb_bytes_t *sb_bytes_create(sb_size_t size)
{
    sb_bytes_t *bytes;
    sb_byte_t  *chunk;

    bytes = sb_alloc(sizeof(sb_bytes_t));
    if (!bytes) {
        return nullptr;
    }


    chunk = sb_alloc(size);
    if (!chunk) {
        sb_free(bytes);
        return nullptr;
    }

    bytes->chunk = chunk;
    bytes->size  = size;

    bytes->r_index = 0;
    bytes->w_index = 0;
    
    bytes->pre_free = 0;
    bytes->pre_size = 0;

    return bytes;
}


sb_void_t sb_bytes_destroy(sb_bytes_t *bytes)
{
    sb_free(bytes->chunk);
    sb_free(bytes);
}


sb_size_t sb_bytes_read(sb_bytes_t *bytes, sb_byte_t *data, sb_size_t size)
{
    sb_size_t readable = sb_bytes_readable(bytes);

    if (size > readable) {
        size = readable;
    }

    if (size) {
        sb_memcopy(data, sb_bytes_peek(bytes), size);
        bytes->r_index += size;
    }

    return size;
}


sb_bool_t sb_bytes_read_int8(sb_bytes_t *bytes, sb_int8_t *value)
{   
    return sb_bytes_readable(bytes) >= sizeof(sb_int8_t) && 
        sb_bytes_read(bytes, (sb_byte_t*)value, sizeof(sb_int8_t));
}


sb_bool_t sb_bytes_read_uint8(sb_bytes_t *bytes, sb_uint8_t *value)
{
    return sb_bytes_readable(bytes) >= sizeof(sb_uint8_t) &&
        sb_bytes_read(bytes, (sb_byte_t*)value, sizeof(sb_uint8_t));
}


sb_bool_t sb_bytes_read_int16(sb_bytes_t *bytes, sb_int16_t *value)
{
    return sb_bytes_readable(bytes) >= sizeof(sb_int16_t) &&
        sb_bytes_read(bytes, (sb_byte_t*)value, sizeof(sb_int16_t));
}


sb_bool_t sb_bytes_read_uint16(sb_bytes_t *bytes, sb_uint16_t *value)
{
    return sb_bytes_readable(bytes) >= sizeof(sb_uint16_t) &&
        sb_bytes_read(bytes, (sb_byte_t*)value, sizeof(sb_uint16_t));
}


sb_bool_t sb_bytes_read_int32(sb_bytes_t *bytes, sb_int32_t *value)
{
    return sb_bytes_readable(bytes) >= sizeof(sb_int32_t) &&
        sb_bytes_read(bytes, (sb_byte_t*)value, sizeof(sb_int32_t));
}


sb_bool_t sb_bytes_read_uint32(sb_bytes_t *bytes, sb_uint32_t *value)
{
    return sb_bytes_readable(bytes) >= sizeof(sb_uint32_t) &&
        sb_bytes_read(bytes, (sb_byte_t*)value, sizeof(sb_uint32_t));
}


sb_bool_t sb_bytes_read_int64(sb_bytes_t *bytes, sb_int64_t *value)
{
    return sb_bytes_readable(bytes) >= sizeof(sb_int64_t) &&
        sb_bytes_read(bytes, (sb_byte_t*)value, sizeof(sb_int64_t));
}


sb_bool_t sb_bytes_read_uint64(sb_bytes_t *bytes, sb_uint64_t *value)
{
    return sb_bytes_readable(bytes) >= sizeof(sb_uint64_t) &&
        sb_bytes_read(bytes, (sb_byte_t*)value, sizeof(sb_uint64_t));
}


sb_size_t sb_bytes_write(sb_bytes_t *bytes, const sb_byte_t *data, sb_size_t size)
{
    if (!__sb_bytes_expand__(bytes, size)) {
        return 0;
    }

    sb_memcopy(bytes->chunk + bytes->w_index, data, size);

    bytes->w_index += size;

    return size;
}


sb_bool_t sb_bytes_write_int8(sb_bytes_t *bytes, sb_int8_t value)
{
    return sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_int8_t)) != 0;
}


sb_bool_t sb_bytes_write_uint8(sb_bytes_t *bytes, sb_uint8_t value)
{
    return sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_uint8_t)) != 0;
}


sb_bool_t sb_bytes_write_int16(sb_bytes_t *bytes, sb_int16_t value)
{
    return sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_int16_t)) != 0;
}


sb_bool_t sb_bytes_write_uint16(sb_bytes_t *bytes, sb_uint16_t value)
{
    return sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_uint16_t)) != 0;
}


sb_bool_t sb_bytes_write_int32(sb_bytes_t *bytes, sb_int32_t value)
{
    return sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_int32_t)) != 0;
}


sb_bool_t sb_bytes_write_uint32(sb_bytes_t *bytes, sb_uint32_t value)
{
    return sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_uint32_t)) != 0;
}


sb_bool_t sb_bytes_write_int64(sb_bytes_t *bytes, sb_int64_t value)
{
    return sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_int64_t)) != 0;
}


sb_bool_t sb_bytes_write_uint64(sb_bytes_t *bytes, sb_uint64_t value)
{
    return sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_uint64_t)) != 0;
}


sb_bool_t sb_bytes_peek_int8(sb_bytes_t *bytes, sb_int8_t *value)
{
    if (sizeof(sb_int8_t) > sb_bytes_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_peek(bytes), sizeof(sb_int8_t));

    return true;
}


sb_bool_t sb_bytes_peek_uint8(sb_bytes_t *bytes, sb_uint8_t *value)
{
    if (sizeof(sb_uint8_t) > sb_bytes_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_peek(bytes), sizeof(sb_uint8_t));

    return true;
}


sb_bool_t sb_bytes_peek_int16(sb_bytes_t *bytes, sb_int16_t *value)
{
    if (sizeof(sb_int16_t) > sb_bytes_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_peek(bytes), sizeof(sb_int16_t));

    return true;
}


sb_bool_t sb_bytes_peek_uint16(sb_bytes_t *bytes, sb_uint16_t *value)
{
    if (sizeof(sb_uint16_t) > sb_bytes_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_peek(bytes), sizeof(sb_uint16_t));

    return true;
}


sb_bool_t sb_bytes_peek_int32(sb_bytes_t *bytes, sb_int32_t *value)
{
    if (sizeof(sb_int32_t) > sb_bytes_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_peek(bytes), sizeof(sb_int32_t));

    return true;
}


sb_bool_t sb_bytes_peek_uint32(sb_bytes_t *bytes, sb_uint32_t *value)
{
    if (sizeof(sb_uint32_t) > sb_bytes_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_peek(bytes), sizeof(sb_uint32_t));

    return true;
}


sb_bool_t sb_bytes_peek_int64(sb_bytes_t *bytes, sb_int64_t *value)
{
    if (sizeof(sb_int64_t) > sb_bytes_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_peek(bytes), sizeof(sb_int64_t));

    return true;
}


sb_bool_t sb_bytes_peek_uint64(sb_bytes_t *bytes, sb_uint64_t *value)
{
    if (sizeof(sb_uint64_t) > sb_bytes_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_peek(bytes), sizeof(sb_uint64_t));

    return true;
}


sb_bytes_t *sb_bytes_dup(const sb_bytes_t *src)
{
    sb_bytes_t *dst;
    sb_size_t   size = sb_bytes_prepend_size(src) + sb_bytes_readable(src);

    if (!(dst = sb_bytes_create(size))) {
        return nullptr;
    }

    sb_bytes_prepend_create(dst, sb_bytes_prepend_size(src));

    sb_bytes_prepend_write(dst, sb_bytes_prepend_peek(src),
        sb_bytes_prepend_readable(src));

    sb_bytes_write(dst, sb_bytes_peek(src), sb_bytes_readable(src));

    return dst;
}


sb_bool_t sb_bytes_shrink(sb_bytes_t *bytes)
{
    sb_bytes_t *tmp;

    if (!(tmp = sb_bytes_dup(bytes))) {
        return false;
    }

    sb_bytes_swap(bytes, tmp);

    sb_bytes_destroy(tmp);

    return true;
}


sb_size_t sb_bytes_truncate(sb_bytes_t *bytes, sb_ssize_t size)
{
    if (size > 0) {
        if ((sb_size_t)size > sb_bytes_writeable(bytes)) {
            if (!__sb_bytes_expand__(bytes, (sb_size_t)size)) {
                return 0;
            }
        }

    } else if (size < 0) {
        sb_size_t readable = sb_bytes_readable(bytes);

        if ((sb_size_t)(-size) > readable) {
            size = -(sb_ssize_t)readable;
        }
    }

    bytes->w_index = (sb_size_t)(bytes->w_index + size);

    return (sb_size_t) size;
}


sb_size_t sb_bytes_range(sb_bytes_t *bytes, sb_int_t start, sb_int_t end,
    sb_byte_t *data, sb_size_t size)
{ 
    sb_size_t readable = sb_bytes_readable(bytes);
    sb_int_t  range_size;

    if (readable == 0) {
        return 0;
    }

    start = start >= 0 ? start : 0;
    end = end >= 0 ? end : readable;
    range_size = end - start;

    if (range_size < 0) {
        return 0;
    }

    size = size > (sb_size_t) range_size ? range_size + 1: size;

    sb_memcopy(data, sb_bytes_peek(bytes) + start, size);

    return size;
}


sb_size_t sb_bytes_retrieve(sb_bytes_t *bytes, sb_size_t size)
{
    sb_size_t readable = sb_bytes_readable(bytes);

    if (size > readable) {
        size = readable;
    }

    bytes->r_index += size;
    return size;
}


sb_bool_t sb_bytes_prepend_create(sb_bytes_t *bytes, sb_size_t size)
{
    sb_size_t prepend_readable = sb_bytes_prepend_readable(bytes);
    sb_size_t move_size = prepend_readable < size ? prepend_readable : size;
    sb_size_t readable = sb_bytes_readable(bytes);

    if (sb_bytes_prependable(bytes) >= size) {
        if (prepend_readable) {
            sb_memmove(sb_bytes_prepend_peek(bytes),
                bytes->chunk + bytes->r_index - size, 
                move_size);
        }

    } else {
        if (sb_bytes_capacity(bytes) >= size) {

            if (prepend_readable) {
                sb_memcopy(bytes->chunk,
                    sb_bytes_prepend_peek(bytes),
                    move_size);
            }

            sb_memmove(bytes->chunk + size,
                sb_bytes_peek(bytes),
                readable);
            
        } else {
            sb_size_t  nalloc;
            sb_byte_t *chunk;

            nalloc = bytes->size * 2;
            nalloc = size + readable > nalloc ? size + readable : nalloc;

            chunk = sb_alloc(nalloc);
            if (!chunk) {
                return false;
            }

            if (prepend_readable) {
                sb_memcopy(chunk,
                    sb_bytes_prepend_peek(bytes),
                    move_size);
            }

            if (readable) {
                sb_memcopy(chunk + size,
                    sb_bytes_peek(bytes),
                    readable);
            }

            sb_free(bytes->chunk);

            bytes->chunk = chunk;
            bytes->size = nalloc;
        }

        bytes->r_index = size;
        bytes->w_index = readable + size;
    }

    bytes->pre_size = size;
    bytes->pre_free = size - move_size;

    return true;
}


sb_size_t sb_bytes_prepend_read(sb_bytes_t *bytes, 
    sb_byte_t *data, sb_size_t size)
{
    sb_size_t prepend_readable = sb_bytes_prepend_readable(bytes);

    if (size > prepend_readable) {
        size = prepend_readable;
    }

    if (size) {
        sb_memcopy(data, sb_bytes_prepend_peek(bytes), size);
        bytes->pre_size -= size;
    }

    return size;
}


sb_bool_t sb_bytes_prepend_read_int8(sb_bytes_t *bytes, sb_int8_t *value)
{
    return sb_bytes_prepend_readable(bytes) >= sizeof(sb_int8_t) && 
        sb_bytes_prepend_read(bytes, (sb_byte_t*)value, sizeof(sb_int8_t));
}


sb_bool_t sb_bytes_prepend_read_uint8(sb_bytes_t *bytes, sb_uint8_t *value)
{
    return sb_bytes_prepend_readable(bytes) >= sizeof(sb_uint8_t) &&
        sb_bytes_prepend_read(bytes, (sb_byte_t*)value, sizeof(sb_uint8_t));
}


sb_bool_t sb_bytes_prepend_read_int16(sb_bytes_t *bytes, sb_int16_t *value)
{
    return sb_bytes_prepend_readable(bytes) >= sizeof(sb_int16_t) &&
        sb_bytes_prepend_read(bytes, (sb_byte_t*)value, sizeof(sb_int16_t));
}


sb_bool_t sb_bytes_prepend_read_uint16(sb_bytes_t *bytes, sb_uint16_t *value)
{
    return sb_bytes_prepend_readable(bytes) >= sizeof(sb_uint16_t) &&
        sb_bytes_prepend_read(bytes, (sb_byte_t*)value, sizeof(sb_uint16_t));
}


sb_bool_t sb_bytes_prepend_read_int32(sb_bytes_t *bytes, sb_int32_t *value)
{
    return sb_bytes_prepend_readable(bytes) >= sizeof(sb_int32_t) &&
        sb_bytes_prepend_read(bytes, (sb_byte_t*)value, sizeof(sb_int32_t));
}


sb_bool_t sb_bytes_prepend_read_uint32(sb_bytes_t *bytes, sb_uint32_t *value)
{
    return sb_bytes_prepend_readable(bytes) >= sizeof(sb_uint32_t) &&
        sb_bytes_prepend_read(bytes, (sb_byte_t*)value, sizeof(sb_uint32_t));
}


sb_bool_t sb_bytes_prepend_read_int64(sb_bytes_t *bytes, sb_int64_t *value)
{
    return sb_bytes_prepend_readable(bytes) >= sizeof(sb_int64_t) &&
        sb_bytes_prepend_read(bytes, (sb_byte_t*)value, sizeof(sb_int64_t));
}


sb_bool_t sb_bytes_prepend_read_uint64(sb_bytes_t *bytes, sb_uint64_t *value)
{
    return sb_bytes_prepend_readable(bytes) >= sizeof(sb_uint64_t) &&
        sb_bytes_prepend_read(bytes, (sb_byte_t*)value, sizeof(sb_uint64_t));
}


sb_bool_t sb_bytes_prepend_peek_int8(sb_bytes_t *bytes, sb_int8_t *value)
{
    if (sizeof(sb_int8_t) > sb_bytes_prepend_readable(bytes)) {
        return false;
    }
    
    sb_memcopy(value, sb_bytes_prepend_peek(bytes), sizeof(sb_int8_t));

    return true;
}


sb_bool_t sb_bytes_prepend_peek_uint8(sb_bytes_t *bytes, sb_uint8_t *value)
{
    if (sizeof(sb_uint8_t) > sb_bytes_prepend_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_prepend_peek(bytes), sizeof(sb_uint8_t));

    return true;
}


sb_bool_t sb_bytes_prepend_peek_int16(sb_bytes_t *bytes, sb_int16_t *value)
{
    if (sizeof(sb_int16_t) > sb_bytes_prepend_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_prepend_peek(bytes), sizeof(sb_int16_t));

    return true;
}


sb_bool_t sb_bytes_prepend_peek_uint16(sb_bytes_t *bytes, sb_uint16_t *value)
{
    if (sizeof(sb_uint16_t) > sb_bytes_prepend_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_prepend_peek(bytes), sizeof(sb_uint16_t));

    return true;
}


sb_bool_t sb_bytes_prepend_peek_int32(sb_bytes_t *bytes, sb_int32_t *value)
{
    if (sizeof(sb_int32_t) > sb_bytes_prepend_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_prepend_peek(bytes), sizeof(sb_int32_t));

    return true;
}


sb_bool_t sb_bytes_prepend_peek_uint32(sb_bytes_t *bytes, sb_uint32_t *value)
{
    if (sizeof(sb_uint32_t) > sb_bytes_prepend_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_prepend_peek(bytes), sizeof(sb_uint32_t));

    return true;
}


sb_bool_t sb_bytes_prepend_peek_int64(sb_bytes_t *bytes, sb_int64_t *value)
{
    if (sizeof(sb_int64_t) > sb_bytes_prepend_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_prepend_peek(bytes), sizeof(sb_int64_t));

    return true;
}


sb_bool_t sb_bytes_prepend_peek_uint64(sb_bytes_t *bytes, sb_uint64_t *value)
{
    if (sizeof(sb_uint64_t) > sb_bytes_prepend_readable(bytes)) {
        return false;
    }

    sb_memcopy(value, sb_bytes_prepend_peek(bytes), sizeof(sb_uint64_t));

    return true;
}


sb_size_t sb_bytes_prepend_write(sb_bytes_t *bytes,
    sb_byte_t *data, sb_size_t size)
{
    sb_size_t prepend_writeable = sb_bytes_prepend_writeable(bytes);

    if (size > prepend_writeable) {
        size = prepend_writeable;
    }

    if (size) {
        sb_memcopy(bytes->chunk + bytes->r_index - prepend_writeable, 
            data, size);

        if ((bytes->pre_free -= size) == 0) {
            bytes->r_index -= bytes->pre_size;
            bytes->pre_size = 0;
        }
    }

    return size;
}


sb_bool_t sb_bytes_prepend_write_int8(sb_bytes_t *bytes, sb_int8_t value)
{
    return sb_bytes_prepend_writeable(bytes) >= sizeof(sb_int8_t) && 
        sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_int8_t));
}


sb_bool_t sb_bytes_prepend_write_uint8(sb_bytes_t *bytes, sb_uint8_t value)
{
    return sb_bytes_prepend_writeable(bytes) >= sizeof(sb_uint8_t) &&
        sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_uint8_t));
}


sb_bool_t sb_bytes_prepend_write_int16(sb_bytes_t *bytes, sb_int16_t value)
{
    return sb_bytes_prepend_writeable(bytes) >= sizeof(sb_int16_t) &&
        sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_int16_t));
}


sb_bool_t sb_bytes_prepend_write_uint16(sb_bytes_t *bytes, sb_uint16_t value)
{
    return sb_bytes_prepend_writeable(bytes) >= sizeof(sb_uint16_t) &&
        sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_uint16_t));
}


sb_bool_t sb_bytes_prepend_write_int32(sb_bytes_t *bytes, sb_int32_t value)
{
    return sb_bytes_prepend_writeable(bytes) >= sizeof(sb_int32_t) &&
        sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_int32_t));
}


sb_bool_t sb_bytes_prepend_write_uint32(sb_bytes_t *bytes, sb_uint32_t value)
{
    return sb_bytes_prepend_writeable(bytes) >= sizeof(sb_uint32_t) &&
        sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_uint32_t));
}


sb_bool_t sb_bytes_prepend_write_int64(sb_bytes_t *bytes, sb_int64_t value)
{
    return sb_bytes_prepend_writeable(bytes) >= sizeof(sb_int64_t) &&
        sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_int64_t));
}


sb_bool_t sb_bytes_prepend_write_uint64(sb_bytes_t *bytes, sb_uint64_t value)
{
    return sb_bytes_prepend_writeable(bytes) >= sizeof(sb_uint64_t) &&
        sb_bytes_write(bytes, (sb_byte_t*)&value, sizeof(sb_uint64_t));
}


sb_size_t sb_bytes_prepend_truncate(sb_bytes_t *bytes, sb_ssize_t size)
{
    if (size > 0) {
        sb_size_t writeable = sb_bytes_prepend_writeable(bytes);

        if ((sb_size_t)size > writeable) {
            size = writeable;
        }

    } else if (size < 0) {
        sb_size_t readable = sb_bytes_prepend_readable(bytes);

        if ((sb_size_t)(-size) > readable) {
            size = -(sb_ssize_t)readable;
        }
    }

    bytes->w_index = (sb_size_t)(bytes->w_index + size);

    return (sb_size_t) size;
}


sb_size_t sb_bytes_prepend_retrieve(sb_bytes_t *bytes, sb_size_t size)
{
    sb_size_t prepend_readable = sb_bytes_prepend_readable(bytes);

    if (size > prepend_readable) {
        size = prepend_readable;
    }

    bytes->pre_size += size;
    return size;
}


static
sb_bool_t __sb_bytes_expand__(sb_bytes_t *bytes, sb_size_t size)
{
    sb_size_t readable = sb_bytes_readable(bytes);
    sb_size_t writeable = sb_bytes_writeable(bytes);
    sb_size_t prepend_size = sb_bytes_prepend_size(bytes);

    if (size > writeable) {
        sb_size_t capacity = sb_bytes_capacity(bytes);
        sb_size_t prepend_readable = sb_bytes_prepend_readable(bytes);

        if (capacity >= size) {
            if (prepend_readable) {
                sb_memmove(bytes->chunk, 
                    sb_bytes_prepend_peek(bytes),
                    prepend_readable);
            }

            if (readable) {
                sb_memmove(bytes->chunk + prepend_size,
                    sb_bytes_peek(bytes),
                    readable);
            }

        } else {
            sb_size_t  nalloc;
            sb_byte_t *chunk;

            nalloc = bytes->size * 2;
            nalloc = size + readable > nalloc ? size + readable : nalloc;

            chunk = sb_alloc(nalloc);
            if (!chunk) {
                return false;
            }

            if (prepend_readable) {
                sb_memcopy(chunk,
                    sb_bytes_prepend_peek(bytes),
                    prepend_readable);
            }

            if (readable) {
                sb_memcopy(chunk + prepend_size,
                    sb_bytes_peek(bytes),
                    readable);
            }

            sb_free(bytes->chunk);

            bytes->chunk = chunk;
            bytes->size = nalloc;
        }

        bytes->r_index = prepend_size;
        bytes->w_index = readable + prepend_size;
    }

    return true;
}
