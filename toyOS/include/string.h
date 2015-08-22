#ifndef _STRING_H
#define _STRING_H

#ifndef NULL
#define	NULL ((void*)0)
#endif 

void* memset(void *buffer, int ch, unsigned int count);
int strlen(const char *s);
char * strcpy(char *dest, const char *src);

#endif 