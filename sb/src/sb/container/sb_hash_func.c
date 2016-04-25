

#include <sb/sb_config.h>
#include <sb/container/sb_hash_func.h>


sb_uint32_t sb_murmur2_hash(sb_byte_t *data, sb_size_t len)
{
    sb_uint32_t  h, k;

    h = 0 ^ len;

    while (len >= 4) {
        k = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= 0x5BD1E995;
        k ^= k >> 24;
        k *= 0x5BD1E995;

        h *= 0x5BD1E995;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len) {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= 0x5BD1E995;
    }

    h ^= h >> 13;
    h *= 0x5BD1E995;
    h ^= h >> 15;

    return h;
}


sb_uint32_t sb_rabin_karp_hash(const sb_byte_t *data, sb_size_t len, sb_uint32_t *pow)
{
    sb_uint32_t     hash = 0;
    sb_uint_t       i = 0, p = 1, sq = SB_HASH_PRIME_RK;

    for (; i < len; i++) {
        hash = hash * SB_HASH_PRIME_RK + (sb_uint32_t)data[i];
    }

    for (i = len; i > 0; i >>= 1) {
        if ((i & 1) != 0) {
            p *= sq;
        }
        sq *= sq;
    }

    if (pow) {
        *pow = p;
    }

    return hash;
}
