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

void itoa(int num, char *str){
	int i = 0;
	int negative = 0;

	if (num == 0){
		str[i++] = '0';
		str[i] = '\0';
		return;
	}
	if (num < 0){
		negative = 1;
		num = -num;
	}

	while (num > 0){
		int rem = num%10;
		str[i++] = rem + '0';
		num /= 10;
	}

	if (negative) {
    	str[i++] = '-';
	}

	str[i] = '\0';
	int start = 0;
	int end = i-1;
	while (start < end){
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
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
