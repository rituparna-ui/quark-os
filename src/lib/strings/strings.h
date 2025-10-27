#ifndef LIB_STRINGS_H
#define LIB_STRINGS_H

#include <stdint.h>

uint32_t strlen(const char *s);
void str_reverse(char s[]);
void int_to_string(int n, char s[]);
char *uint_to_string(uint32_t n, char s[]);

#endif
