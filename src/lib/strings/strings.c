#include "string.h"
#include <stdint.h>

char *uint32t_to_string(uint32_t value) {
  char *kek = "Hello";
  int i = 0;
  while (value) {
    int dig = value % 10;
    value /= 10;
    kek[i++] = dig + '0';
  }
  kek[i] = '\n';
  return kek;
}
