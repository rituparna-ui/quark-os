#include "heap.h"
#include <stdint.h>

struct SimpleHeap {
  uint64_t start;
  uint64_t end;

  uint64_t avail;

  uint64_t free_ptr;
} simple_heap;

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

void simple_heap_init() {
  __asm__ volatile("ldr %0, =_heap_start" : "=r"(simple_heap.start));
  __asm__ volatile("ldr %0, =_heap_end" : "=r"(simple_heap.end));

  simple_heap.avail = simple_heap.end - simple_heap.start;
  simple_heap.free_ptr = simple_heap.start;

  char buff[20];

  uart_println("Heap Initialized");
  uart_puts("Heap Starts: ");
  uint32_to_hex_string(simple_heap.start, buff);
  uart_println(buff);

  uart_puts("Free Ptr: ");
  uint32_to_hex_string(simple_heap.free_ptr, buff);
  uart_println(buff);

  uart_puts("Heap Ends: ");
  uint32_to_hex_string(simple_heap.end, buff);
  uart_println(buff);

  return;
}

uint32_t get_32bytes_aligned_bytes(uint32_t bytes) {
  if (bytes == 0) {
    return 0;
  }

  uint32_t num_blocks = (bytes + 32 - 1) / 32;
  uint32_t aligned_bytes = num_blocks * 32;

  return aligned_bytes;
}

void *simple_heap_alloc(uint32_t bytes) {
  uart_println("");
  uart_println("Before Allocating: ");
  char buff[20];

  if (bytes > simple_heap.avail) {
    uart_errorln("Cannot allocate. Heap full.");
    return 0;
  }

  uart_puts("Free Ptr: ");
  uint32_to_hex_string(simple_heap.free_ptr, buff);
  uart_println(buff);

  void *addr = (void *)simple_heap.free_ptr;

  uint32_t aligned_bytes = get_32bytes_aligned_bytes(bytes);

  simple_heap.free_ptr += aligned_bytes;

  uart_println("After Allocating: ");
  uart_puts("Free Ptr: ");
  uint32_to_hex_string(simple_heap.free_ptr, buff);
  uart_println(buff);

  return addr;
}
