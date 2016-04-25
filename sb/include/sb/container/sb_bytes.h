

#ifndef __SB__BUFFER__H__INCLUDE__
#define __SB__BUFFER__H__INCLUDE__

#include <sb/sb_config.h>
#include <sb/memory/sb_strops.h>

typedef struct sb_bytes_s {
    sb_byte_t* chunk;

    sb_size_t  r_index;
    sb_size_t  w_index;

    sb_size_t  pre_free;
    sb_size_t  pre_size;

    sb_size_t  size; 
}sb_bytes_t;

static sb_inline
sb_void_t sb_bytes_swap(sb_bytes_t *lhs, sb_bytes_t *rhs)
{
    sb_bytes_t tmp = *lhs;
    *lhs = *rhs;
    *rhs = tmp;
}

#define sb_bytes_prepend_size(bytes)                                          \
    ((bytes)->pre_size)

#define sb_bytes_prepend_readable(bytes)                                      \
    ((bytes)->pre_size - (bytes)->pre_free)

#define sb_bytes_prepend_writeable(bytes)                                     \
    ((bytes)->pre_free)

#define sb_bytes_prepend_peek(bytes)                                          \
    ((bytes)->pre_size ?                                                      \
        (bytes)->chunk + (bytes)->r_index - (bytes)->pre_size : nullptr)

#define sb_bytes_prepend_merge(bytes)                                         \
    do {                                                                      \
        (bytes)->r_index -= (bytes)->pre_size;                                \
        (bytes)->pre_size = 0;                                                \
        (bytes)->pre_free = 0;                                                \
    } while(false)

#define sb_bytes_prepend_destroy(bytes)                                       \
    do {                                                                      \
        (bytes)->pre_size = 0;                                                \
        (bytes)->pre_free = 0;                                                \
    } while(false)

#define sb_bytes_readable(bytes)                                              \
    ((bytes)->w_index - (bytes)->r_index)

#define sb_bytes_writeable(bytes)                                             \
    ((bytes)->size - (bytes)->w_index)

#define sb_bytes_prependable(bytes)                                           \
    ((bytes)->r_index)

#define sb_bytes_capacity(bytes)                                              \
    (sb_bytes_writeable(bytes) + sb_bytes_prependable(bytes) -                \
        sb_bytes_prepend_size(bytes))

#define sb_bytes_size(bytes)                                                  \
    ((bytes)->size)

#define sb_bytes_peek(bytes)                                                  \
    ((bytes)->chunk + (bytes)->r_index)

sb_bytes_t *sb_bytes_create(sb_size_t size);
sb_void_t sb_bytes_destroy(sb_bytes_t *bytes);
sb_bool_t sb_bytes_shrink(sb_bytes_t *bytes);
sb_bytes_t *sb_bytes_dup(const sb_bytes_t *src);
sb_size_t sb_bytes_truncate(sb_bytes_t *bytes, sb_ssize_t size);
sb_size_t sb_bytes_retrieve(sb_bytes_t *bytes, sb_size_t size);
sb_size_t sb_bytes_range(sb_bytes_t *bytes, 
    sb_int_t start, sb_int_t end, sb_byte_t *data, sb_size_t size);
sb_size_t sb_bytes_read(sb_bytes_t *bytes, sb_byte_t *data, sb_size_t size);
sb_bool_t sb_bytes_read_int8(sb_bytes_t *bytes, sb_int8_t *value);
sb_bool_t sb_bytes_read_uint8(sb_bytes_t *bytes, sb_uint8_t *value);
sb_bool_t sb_bytes_read_int16(sb_bytes_t *bytes, sb_int16_t *value);
sb_bool_t sb_bytes_read_uint16(sb_bytes_t *bytes, sb_uint16_t *value);
sb_bool_t sb_bytes_read_int32(sb_bytes_t *bytes, sb_int32_t *value);
sb_bool_t sb_bytes_read_uint32(sb_bytes_t *bytes, sb_uint32_t *value);
sb_bool_t sb_bytes_read_int64(sb_bytes_t *bytes, sb_int64_t *value);
sb_bool_t sb_bytes_read_uint64(sb_bytes_t *bytes, sb_uint64_t *value);
sb_bool_t sb_bytes_peek_int8(sb_bytes_t *bytes, sb_int8_t *value);
sb_bool_t sb_bytes_peek_uint8(sb_bytes_t *bytes, sb_uint8_t *value);
sb_bool_t sb_bytes_peek_int16(sb_bytes_t *bytes, sb_int16_t *value);
sb_bool_t sb_bytes_peek_uint16(sb_bytes_t *bytes, sb_uint16_t *value);
sb_bool_t sb_bytes_peek_int32(sb_bytes_t *bytes, sb_int32_t *value);
sb_bool_t sb_bytes_peek_uint32(sb_bytes_t *bytes, sb_uint32_t *value);
sb_bool_t sb_bytes_peek_int64(sb_bytes_t *bytes, sb_int64_t *value);
sb_bool_t sb_bytes_peek_uint64(sb_bytes_t *bytes, sb_uint64_t *value);
sb_size_t sb_bytes_write(sb_bytes_t *bytes,
    const sb_byte_t *data, sb_size_t size);
sb_bool_t sb_bytes_write_int8(sb_bytes_t *bytes, sb_int8_t value);
sb_bool_t sb_bytes_write_uint8(sb_bytes_t *bytes, sb_uint8_t value);
sb_bool_t sb_bytes_write_int16(sb_bytes_t *bytes, sb_int16_t value);
sb_bool_t sb_bytes_write_uint16(sb_bytes_t *bytes, sb_uint16_t value);
sb_bool_t sb_bytes_write_int32(sb_bytes_t *bytes, sb_int32_t value);
sb_bool_t sb_bytes_write_uint32(sb_bytes_t *bytes, sb_uint32_t value);
sb_bool_t sb_bytes_write_int64(sb_bytes_t *bytes, sb_int64_t value);
sb_bool_t sb_bytes_write_uint64(sb_bytes_t *bytes, sb_uint64_t value);
sb_bool_t sb_bytes_prepend_create(sb_bytes_t *bytes, sb_size_t size);
sb_size_t sb_bytes_prepend_truncate(sb_bytes_t *bytes, sb_ssize_t size);
sb_size_t sb_bytes_prepend_retrieve(sb_bytes_t *bytes, sb_size_t size);
sb_size_t sb_bytes_prepend_read(sb_bytes_t *bytes,
    sb_byte_t *data, sb_size_t size);
sb_bool_t sb_bytes_prepend_read_int8(sb_bytes_t *bytes, sb_int8_t *value);
sb_bool_t sb_bytes_prepend_read_uint8(sb_bytes_t *bytes, sb_uint8_t *value);
sb_bool_t sb_bytes_prepend_read_int16(sb_bytes_t *bytes, sb_int16_t *value);
sb_bool_t sb_bytes_prepend_read_uint16(sb_bytes_t *bytes, sb_uint16_t *value);
sb_bool_t sb_bytes_prepend_read_int32(sb_bytes_t *bytes, sb_int32_t *value);
sb_bool_t sb_bytes_prepend_read_uint32(sb_bytes_t *bytes, sb_uint32_t *value);
sb_bool_t sb_bytes_prepend_read_int64(sb_bytes_t *bytes, sb_int64_t *value);
sb_bool_t sb_bytes_prepend_read_uint64(sb_bytes_t *bytes, sb_uint64_t *value);
sb_bool_t sb_bytes_prepend_peek_int8(sb_bytes_t *bytes, sb_int8_t *value);
sb_bool_t sb_bytes_prepend_peek_uint8(sb_bytes_t *bytes, sb_uint8_t *value);
sb_bool_t sb_bytes_prepend_peek_int16(sb_bytes_t *bytes, sb_int16_t *value);
sb_bool_t sb_bytes_prepend_peek_uint16(sb_bytes_t *bytes, sb_uint16_t *value);
sb_bool_t sb_bytes_prepend_peek_int32(sb_bytes_t *bytes, sb_int32_t *value);
sb_bool_t sb_bytes_prepend_peek_uint32(sb_bytes_t *bytes, sb_uint32_t *value);
sb_bool_t sb_bytes_prepend_peek_int64(sb_bytes_t *bytes, sb_int64_t *value);
sb_bool_t sb_bytes_prepend_peek_uint64(sb_bytes_t *bytes, sb_uint64_t *value);
sb_size_t sb_bytes_prepend_write(sb_bytes_t *bytes,
    sb_byte_t *data, sb_size_t size);
sb_bool_t sb_bytes_prepend_write_int8(sb_bytes_t *bytes, sb_int8_t value);
sb_bool_t sb_bytes_prepend_write_uint8(sb_bytes_t *bytes, sb_uint8_t value);
sb_bool_t sb_bytes_prepend_write_int16(sb_bytes_t *bytes, sb_int16_t value);
sb_bool_t sb_bytes_prepend_write_uint16(sb_bytes_t *bytes, sb_uint16_t value);
sb_bool_t sb_bytes_prepend_write_int32(sb_bytes_t *bytes, sb_int32_t value);
sb_bool_t sb_bytes_prepend_write_uint32(sb_bytes_t *bytes, sb_uint32_t value);
sb_bool_t sb_bytes_prepend_write_int64(sb_bytes_t *bytes, sb_int64_t value);
sb_bool_t sb_bytes_prepend_write_uint64(sb_bytes_t *bytes, sb_uint64_t value);

#endif
