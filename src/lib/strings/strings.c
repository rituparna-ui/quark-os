#include "strings.h"
#include "../../heap/heap.h"

char *uint32_to_hex_string(uint32_t value) {
  const int num_digits = 8;
  const int prefix_len = 2;
  const int total_len = prefix_len + num_digits;

  char *buffer = (char *)simple_heap_alloc(total_len + 1);
  if (!buffer) {
    return "ERR";
  }

  buffer[0] = '0';
  buffer[1] = 'x';

  for (int i = 0; i < num_digits; i++) {
    int shift = (num_digits - 1 - i) * 4;
    uint32_t digit = (value >> shift) & 0xF;

    if (digit < 10) {

      buffer[prefix_len + i] = '0' + digit;
    } else {
      buffer[prefix_len + i] = 'A' + (digit - 10);
    }
  }

  buffer[total_len] = '\0';
  return buffer;
}

char *uint32_to_dec_string(uint32_t value) {
  const int max_digits = 10;

  char *buffer = (char *)simple_heap_alloc(max_digits + 1);
  if (!buffer) {
    return "ERR";
  }

  if (value == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return buffer;
  }

  int pos = max_digits;
  buffer[pos] = '\0';

  while (value > 0) {
    uint32_t digit = value % 10;
    value /= 10;
    buffer[--pos] = '0' + digit;
  }

  int start = 0;
  while (buffer[pos]) {
    buffer[start++] = buffer[pos++];
  }
  buffer[start] = '\0';

  return buffer;
}

char *uint32_to_bin_string(uint32_t value) {
  const int num_bits = 32;
  const int prefix_len = 2;
  const int total_len = prefix_len + num_bits;

  char *buffer = (char *)simple_heap_alloc(total_len + 1);
  if (!buffer) {
    return "ERR";
  }

  buffer[0] = '0';
  buffer[1] = 'b';

  for (int i = 0; i < num_bits; i++) {
    int shift = (num_bits - 1 - i);
    uint32_t bit = (value >> shift) & 1;
    buffer[prefix_len + i] = bit ? '1' : '0';
  }

  buffer[total_len] = '\0';
  return buffer;
}
