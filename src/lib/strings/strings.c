#include "strings.h"
#include <stdint.h>

void str_reverse(char s[]) {
  int i = 0;
  int j = 0;
  char temp;

  while (s[j] != '\0') {
    j++;
  }
  j--;

  while (i < j) {
    temp = s[i];
    s[i] = s[j];
    s[j] = temp;
    i++;
    j--;
  }
}

void int_to_string(int n, char s[]) {
  int i = 0;
  int sign;

  if ((sign = n) < 0) {
    unsigned long long temp_n =
        (unsigned long long)(n == -2147483648 ? 2147483648UL : -n);
    n = (int)temp_n;
  }

  do {
    s[i++] = (n % 10) + '0';
  } while ((n /= 10) > 0);

  if (sign < 0) {
    s[i++] = '-';
  }

  s[i] = '\0';

  str_reverse(s);
}

char *uint_to_string(uint32_t n, char s[]) {
  int i = 0;

  do {
    s[i++] = (n % 10) + '0';
  } while ((n /= 10) > 0);

  s[i] = '\0';

  str_reverse(s);

  return s;
}

uint32_t strlen(const char *s) {
  uint32_t len = 0;

  while (s[len]) {
    len++;
  }

  return len;
}
