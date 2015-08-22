#include <stdarg.h>
#include <stdio.h>

#define UPPER	1
#define LOWER	2
#define SIGNED	4
#define UNSIGNED 8
static char* integer_to_string(unsigned int value, char *string, int len, int radix, int flags)
{
	char table_lower[] = {"0123456789abcdef"};
	char table_upper[] = {"0123456789ABCDEF"};

	char ret[11] = {0};
	int i = 0;
	int j = 0;

	if (value == 0) {
		string[0] = '0';
		string[1] = 0;
		return string;
	}
	
	if ((value & 0x80000000) && (flags & SIGNED)) {
		string[j++] = '-';
		value = ~value + 1;
	}

	while (value) {
		if (flags == UPPER)
			ret[i++] = table_upper[value % radix];
		else 
			ret[i++] = table_lower[value % radix];
		value /= radix;
	}

	while (i >= 0 && j < len)
		string[j++] = ret[--i];

	string[j-1] = 0;

	return string;

}

int vsprintf(char* string, const char *format, va_list arg_list)
{
	int i;
	int j;
	int len = strlen(format);
	char buf[256] = {0};

	for (i = 0, j = 0; i < len;  i++, j++) {
		string[j] = format[i];
		if (format[i] == '%') {
			switch (format[++i]) {
			case 'i':
			case 'd':
				integer_to_string(va_arg(arg_list, int), 	\
					buf, sizeof(buf), 10, LOWER | SIGNED);
				strcpy(string+j, buf);
				j += strlen(buf)-1;
				break;
			case 'u':
				integer_to_string(va_arg(arg_list, int), 	\
					buf, sizeof(buf), 10, LOWER | UNSIGNED);
				strcpy(string+j, buf);
				j += strlen(buf)-1;
				break;
			case 'o':
				integer_to_string(va_arg(arg_list, int), 	\
				buf, sizeof(buf), 8, LOWER | SIGNED);
				strcpy(string+j, buf);
				j += strlen(buf)-1;
				break;
			case 'x':
				integer_to_string(va_arg(arg_list, int), 	\
					buf, sizeof(buf), 16, LOWER);
				strcpy(string+j, buf);
				j += strlen(buf)-1;
				break;
			case 'X':
				integer_to_string(va_arg(arg_list, int), 	\
					buf, sizeof(buf), 16, UPPER);
				strcpy(string+j, buf);
				j += strlen(buf)-1;
				break;
			case 'c':
				string[j] = (char)(va_arg(arg_list, char));
				break;
			case 's':
				strcpy(string+j, (char *)va_arg(arg_list, char *));
				j += strlen(string+j)-1;
				break;
			case '%':											
				string[j] = '%';
				break;
			}
		}
	}

	return (strlen(string));
}


int sprintf(char* string, const char* format, ...)
{
	va_list arg_list;

	va_start(arg_list, format);

	vsprintf(string, format, arg_list);

	va_end(arg_list);

	return (strlen(string));
}

int printf(char *fmt, ...)
{
	char string[256] = {0};
	va_list arg_list;

	va_start(arg_list, fmt);

	vsprintf(string, fmt, arg_list);
	write(string, strlen(string));

	va_end(arg_list);
	
	return (strlen(string));
}