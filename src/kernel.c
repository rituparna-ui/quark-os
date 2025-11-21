#include "lib/uart/uart.h"

void uint32_to_hex_string(uint32_t value, char *buffer) {
  const int num_digits = 8;

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
}

void kernel_main() {
  uart_init();

  char buffer[20];
  uint32_t heap_start = 0;
  uint32_t heap_end = 0;

  __asm__ volatile("ldr %0, =_heap_start" : "=r"(heap_start)::"memory");
  __asm__ volatile("ldr %0, =_heap_end" : "=r"(heap_end)::"memory");

  uart_puts("Heap start address: ");
  uint32_to_hex_string(heap_start, buffer);
  uart_println(buffer);

  uart_puts("Heap end address: ");
  uint32_to_hex_string(heap_end, buffer);
  uart_println(buffer);

  while (1) {
  }
}
