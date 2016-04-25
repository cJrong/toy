

#ifndef __SB__CONFIG__H__INCLUDE__
#define __SB__CONFIG__H__INCLUDE__


#define SB_WIN32

#ifndef SB_VERSION
#define SB_VERSION      0x000100
#endif

#ifndef SB_ALIGNMENT
#define SB_ALIGNMENT sizeof(unsigned long)
#endif

#ifndef sb_inline
#define sb_inline       
#endif

#ifndef sb_extern
#define sb_extern       extern
#endif

#ifndef sb_cdecl
#define sb_cdecl        __cdecl
#endif

#ifndef sb_stdcall
#define sb_stdcall      __stdcall
#endif

#ifndef sb_fastcall
#define sb_fastcall     __fastcall
#endif


#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

typedef int8_t      sb_int8_t;
typedef uint8_t     sb_uint8_t;
typedef int16_t     sb_int16_t;
typedef uint16_t    sb_uint16_t;
typedef int32_t     sb_int32_t;
typedef uint32_t    sb_uint32_t;
typedef int64_t     sb_int64_t;
typedef uint64_t    sb_uint64_t;

typedef uintptr_t   sb_uintptr_t;
typedef intptr_t    sb_intptr_t;
typedef size_t      sb_size_t;
typedef long long     sb_ssize_t;

#ifndef sb_void_t
#define sb_void_t       void
#endif

typedef char            sb_char_t;
typedef unsigned char   sb_uchar_t;
typedef short           sb_short_t;
typedef unsigned short  sb_ushort_t;
typedef int             sb_int_t;
typedef unsigned int    sb_uint_t;
typedef long            sb_long_t;
typedef unsigned long   sb_ulong_t;

typedef float           sb_float32_t;
typedef double          sb_float64_t;
typedef float           sb_float_t;
typedef double          sb_double_t;

typedef sb_uint8_t      sb_byte_t;
typedef sb_uint16_t     sb_word_t;
typedef sb_uint32_t     sb_dword_t;

typedef sb_void_t*      sb_voidp_t;
typedef sb_byte_t*      sb_bytep_t;
typedef sb_word_t*      sb_wordp_t;
typedef sb_dword_t*     sb_dwordp_t;


typedef sb_int8_t       sb_bool_t;

#ifndef false
#define false (0)
#endif

#ifndef true
#define true (!false)
#endif

#ifndef nullptr
#define nullptr (0)
#endif


#define sb_declare_block_inner_variable(type, value_name)                      \
    type __sb__block__inner__variable__ ## value_name ## __

#define sb_acquire_block_inner_variable(value_name)                           \
    __sb__block__inner__variable__ ## value_name ## __

#endif
