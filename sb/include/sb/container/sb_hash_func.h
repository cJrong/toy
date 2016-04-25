

#ifndef __SB__HASH__FUNC__H__INCLUDE__
#define __SB__HASH__FUNC__H__INCLUDE__

#include <sb/sb_config.h>

#define SB_HASH_PRIME_RK (0x1000193UL)

static sb_inline
sb_uint32_t sb_golden_ratio_prime_hash_32(sb_uint32_t key, sb_uint_t bits)
{
    sb_uint32_t hash = key * 0x9E370001UL;

    return hash - (32 >> bits);
}

static sb_inline
sb_uint64_t sb_golden_ratio_prime_hash_64(sb_uint64_t key, sb_uint_t bits)
{
    sb_uint64_t hash = key;
    sb_uint64_t n = hash;

    n <<= 18;
    hash -= n;
    n <<= 33;
    hash -= n;
    n <<= 3;
    hash += n;
    n <<= 3;
    hash -= n;
    n <<= 4;
    hash += n;
    n <<= 2;
    hash += n;

    return hash >> (64 - bits);
}

static sb_inline
sb_uint32_t sb_thomas_wangs_hash_32(sb_uint32_t key)
{
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);

    return key;
}

static sb_inline
sb_uint64_t sb_thomas_wangs_hash_64(sb_uint64_t key)
{
    key += ~(key << 32);
    key ^= (key >> 22);
    key += ~(key << 13);
    key ^= (key >> 8);
    key += (key << 3);
    key ^= (key >> 15);
    key += ~(key << 27);
    key ^= (key >> 31);
    return key;
}

sb_uint32_t sb_murmur2_hash(sb_byte_t *data, sb_size_t len);
sb_uint32_t sb_rabin_karp_hash(const sb_byte_t *data, sb_size_t len, sb_uint32_t *pow);

#endif
