#include "strings.h"
#include "../../heap/heap.h"

char *uint32_to_hex_string(uint32_t value) {
  const int num_digits = 8;
  char *buffer = (char *)simple_heap_alloc(8);

  for (int i = 0; i < num_digits; i++) {
    int shift = (num_digits - 1 - i) * 4;

    uint32_t digit_value = (value >> shift) & 0xF;

    if (digit_value < 10) {
      buffer[i] = digit_value + '0';
    } else {
      buffer[i] = digit_value + 'A' - 10;
    }
  }

  buffer[num_digits] = '\0';

  return buffer;
}
