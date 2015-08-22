#ifndef _STDIO_H
#define _STDIO_H
#include <stdarg.h>

#ifndef NULL
#define	NULL ((void*)0)
#endif 

int sprintf(char* string, const char* format, ...);
int vsprintf(char* string, const char *format, va_list arg_list);
int printk(char *fmt, ...);

#endif 