#include "string.h"

size_t strlen(const char *s)
{
	size_t n = 0;
	while (s[n]) n++;
	return n;
}

int streq(const char *s1, const char *s2){
	while (*s1 && *s2){
		if (*s1 != *s2){
			return 0;
		}
		s1++;
		s2++;
	}

	return (*s1 == '\0' && *s2 == '\0');
}

void *memset(void *dst, int c, size_t n)
{
	unsigned char *d = dst;
	while (n--) *d++ = (unsigned char)c;
	return dst;
}

void *memcpy(void *dst, const void *src, size_t n)
{
	unsigned char *d = dst;
	const unsigned char *s = src;
	while (n--) *d++ = *s++;
	return dst;
}
