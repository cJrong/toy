

#ifndef __SB__TUPLE__H__INCLUDE__
#define __SB__TUPLE__H__INCLUDE__

#define sb_tuple_t(TUPLEID, vs)                                               \
     struct __sb__ ## TUPLEID ## __ ## vs ## __s__

#define sb_tuple_get(TUPLEID, value, index)                                   \
    value.__##TUPLEID##__V##index##__

#define sb_tuple1_id(TUPLEID, T0)                                             \
    struct __sb__ ## TUPLEID ## __1__s__ {                                    \
        T0 __ ## TUPLEID ## __V0__;}

#define sb_tuple1_t(TUPLEID)                                                  \
    struct __sb__ ## TUPLEID ## __1__s__ 

#define sb_tuple1_make(TUPLEID, VALUENAME, V0)                                \
    sb_tuple1_t(TUPLEID) VALUENAME;                                           \
    VALUENAME.__ ## TUPLEID ## __V0__ = V0;                                   \

#define return_tuple1(TUPLEID, V0)                                            \
   {sb_tuple1_t(TUPLEID) __ ## TUPLEID ## __VALUE__;                          \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V0__ = V0;                  \
    return __ ## TUPLEID ## __VALUE__;}

#define sb_tuple2_id(TUPLEID, T0, T1)                                         \
    struct __sb__ ## TUPLEID ## __2__s__ {                                    \
        T0 __ ## TUPLEID ## __V0__;                                           \
        T1 __ ## TUPLEID ## __V1__;}

#define sb_tuple2_t(TUPLEID)                                                  \
    struct __sb__ ## TUPLEID ## __2__s__ 

#define sb_tuple2_make(TUPLEID, VALUENAME, V0, V1)                            \
    sb_tuple2_t(TUPLEID) VALUENAME;                                           \
    VALUENAME.__ ## TUPLEID ## __V0__ = V0;                                   \
    VALUENAME.__ ## TUPLEID ## __V1__ = V1;

#define return_tuple2(TUPLEID, V0, V1)                                        \
   {sb_tuple2_t(TUPLEID) __ ## TUPLEID ## __VALUE__;                          \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V0__ = V0;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V1__ = V1;                  \
    return __ ## TUPLEID ## __VALUE__;}

#define sb_tuple3_id(TUPLEID, T0, T1, T2)                                     \
    struct __sb__ ## TUPLEID ## __3__s__ {                                    \
        T0 __ ## TUPLEID ## __V0__;                                           \
        T1 __ ## TUPLEID ## __V1__;                                           \
        T2 __ ## TUPLEID ## __V2__;}

#define sb_tuple3_t(TUPLEID)                                                  \
    struct __sb__ ## TUPLEID ## __3__s__ 

#define sb_tuple3_make(TUPLEID, VALUENAME, V0, V1, V2)                        \
    sb_tuple3_t(TUPLEID) VALUENAME;                                           \
    VALUENAME.__ ## TUPLEID ## __V0__ = V0;                                   \
    VALUENAME.__ ## TUPLEID ## __V1__ = V1;                                   \
    VALUENAME.__ ## TUPLEID ## __V2__ = V2;                                   

#define return_tuple3(TUPLEID, V0, V1, V2)                                    \
   {sb_tuple3_t(TUPLEID) __ ## TUPLEID ## __VALUE__;                          \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V0__ = V0;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V1__ = V1;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V2__ = V2;                  \
    return __ ## TUPLEID ## __VALUE__;}

#define sb_tuple4_id(TUPLEID, T0, T1, T2, T3)                                 \
    struct __sb__ ## TUPLEID ## __4__s__ {                                    \
        T0 __ ## TUPLEID ## __V0__;                                           \
        T1 __ ## TUPLEID ## __V1__;                                           \
        T2 __ ## TUPLEID ## __V2__;                                           \
        T3 __ ## TUPLEID ## __V3__;}

#define sb_tuple4_t(TUPLEID)                                                  \
    struct __sb__ ## TUPLEID ## __4__s__ 

#define sb_tuple4_make(TUPLEID, VALUENAME, V0, V1, V2, V3)                    \
    sb_tuple4_t(TUPLEID) VALUENAME;                                           \
    VALUENAME.__ ## TUPLEID ## __V0__ = V0;                                   \
    VALUENAME.__ ## TUPLEID ## __V1__ = V1;                                   \
    VALUENAME.__ ## TUPLEID ## __V2__ = V2;                                   \
    VALUENAME.__ ## TUPLEID ## __V3__ = V3;                                   

#define return_tuple4(TUPLEID, V0, V1, V2, V3)                                \
   {sb_tuple4_t(TUPLEID) __ ## TUPLEID ## __VALUE__;                          \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V0__ = V0;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V1__ = V1;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V2__ = V2;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V3__ = V3;                  \
    return __ ## TUPLEID ## __VALUE__;}

#define sb_tuple5_id(TUPLEID, T0, T1, T2, T3, T4)                             \
    struct __sb__ ## TUPLEID ## __5__s__ {                                    \
        T0 __ ## TUPLEID ## __V0__;                                           \
        T1 __ ## TUPLEID ## __V1__;                                           \
        T2 __ ## TUPLEID ## __V2__;                                           \
        T3 __ ## TUPLEID ## __V3__;                                           \
        T4 __ ## TUPLEID ## __V4__; }

#define sb_tuple5_t(TUPLEID)                                                  \
    struct __sb__ ## TUPLEID ## __5__s__ 

#define sb_tuple5_make(TUPLEID, VALUENAME, V0, V1, V2, V3, V4)                \
    sb_tuple5_t(TUPLEID) VALUENAME;                                           \
    VALUENAME.__ ## TUPLEID ## __V0__ = V0;                                   \
    VALUENAME.__ ## TUPLEID ## __V1__ = V1;                                   \
    VALUENAME.__ ## TUPLEID ## __V2__ = V2;                                   \
    VALUENAME.__ ## TUPLEID ## __V3__ = V3;                                   \
    VALUENAME.__ ## TUPLEID ## __V4__ = V4;

#define return_tuple5(TUPLEID, V0, V1, V2, V3, V4)                            \
   {sb_tuple5_t(TUPLEID) __ ## TUPLEID ## __VALUE__;                          \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V0__ = V0;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V1__ = V1;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V2__ = V2;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V3__ = V3;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V4__ = V4;                  \
    return __ ## TUPLEID ## __VALUE__;}
    
#define sb_tuple6_id(TUPLEID, T0, T1, T2, T3, T4, T5)                         \
    struct __sb__ ## TUPLEID ## __6__s__ {                                    \
        T0 __ ## TUPLEID ## __V0__;                                           \
        T1 __ ## TUPLEID ## __V1__;                                           \
        T2 __ ## TUPLEID ## __V2__;                                           \
        T3 __ ## TUPLEID ## __V3__;                                           \
        T3 __ ## TUPLEID ## __V4__;                                           \
        T4 __ ## TUPLEID ## __V5__; }

#define sb_tuple6_t(TUPLEID)                                                  \
    struct __sb__ ## TUPLEID ## __6__s__ 

#define sb_tuple6_make(TUPLEID, VALUENAME, V0, V1, V2, V3, V4, V5)            \
    sb_tuple6_t(TUPLEID) VALUENAME;                                           \
    VALUENAME.__ ## TUPLEID ## __V0__ = V0;                                   \
    VALUENAME.__ ## TUPLEID ## __V1__ = V1;                                   \
    VALUENAME.__ ## TUPLEID ## __V2__ = V2;                                   \
    VALUENAME.__ ## TUPLEID ## __V3__ = V3;                                   \
    VALUENAME.__ ## TUPLEID ## __V4__ = V4;                                   \
    VALUENAME.__ ## TUPLEID ## __V5__ = V5;

#define return_tuple6(TUPLEID, V0, V1, V2, V3, V4, V5)                        \
    {sb_tuple6_t(TUPLEID) __ ## TUPLEID ## __VALUE__;                         \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V0__ = V0;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V1__ = V1;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V2__ = V2;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V3__ = V3;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V4__ = V4;                  \
    __ ## TUPLEID ## __VALUE__.__ ## TUPLEID ## __V5__ = V5;                  \
    return __ ## TUPLEID ## __VALUE__;}

#endif