#ifndef STRING_H
#define STRING_H

#include <stddef.h>

typedef struct {
    char* data;
    int length;
} String;

String str_make(char *raw);
int str_equals(String a, String b);
String substring(String str, int start, int len);


// standard char (does not use String type defined above)
size_t strlen(const char *s);
int streq(const char *s1, const char *s2);
void itoa(int num, char *str);
void  *memset(void *dst, int c, size_t n);
void  *memcpy(void *dst, const void *src, size_t n);

#endif
