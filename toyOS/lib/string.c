#include <string.h>

void* memset(void *buffer, int ch, unsigned int count)
{
	int i;
	char *s = (char *)buffer;
	
	if (buffer == NULL)
		return NULL;
	
	for (i = 0; i < count; i++) {
		s[i] = ch;
	}
	
	return s;
}


int strlen(const char *s)
{
	int i=0;
	while (s[++i]);
	return i;
}

char * strcpy(char *dest, const char *src)
{
	char *d = dest;
	while (*d++ = *src++);
	return dest;
}

