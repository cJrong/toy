

#include <sb/sb_config.h>
#include <sb/math/sb_random.h>
#include <stdlib.h>


sb_void_t sb_srand(sb_uint_t seed)
{
    srand(seed);
}


sb_int_t sb_rand(sb_void_t)
{
    return rand();
}
