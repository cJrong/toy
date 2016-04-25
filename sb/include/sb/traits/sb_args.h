

#ifndef __SB__ARGS__H__INCLUDE__
#define __SB__ARGS__H__INCLUDE__

#include <sb/sb_config.h>

#if     defined(SB_USE_BUILTIN_VA_LIST)

typedef __builtin_va_list sb_vargs_t;

#define sb_vargs_start(ap, v)     __builtin_va_start(ap, v)

#define sb_vargs_arg(ap, type)    __builtin_va_arg(ap, type)

#define sb_vargs_end(ap)          __builtin_va_end(ap)

#elif   defined(SB_USE_STDARG_H)

#include <stdarg.h>

typedef va_list sb_vargs_t;

#define sb_vargs_start   va_start

#define sb_vargs_arg     va_arg

#define sb_vargs_end     va_end

#else

typedef sb_bytep_t sb_vargs_t;

#define __sb_rounded_size__(type)                                             \
     (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define sb_vargs_start(ap, v)                                                 \
	((sb_void_t)(ap = (sb_vargs_t)&v + __sb_rounded_size__(v)))

#define sb_vargs_arg(ap, type)                                                \
    (ap += __sb_rounded_size__(type), *(type*)(ap-__sb_rounded_size__(type)))

#define sb_vargs_end(ap)                                                      \
    ((sb_void_t)(ap = nullptr))

#endif

#endif
