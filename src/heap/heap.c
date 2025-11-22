#include "heap.h"

struct SimpleHeap {
  uint64_t start;
  uint64_t end;

  uint64_t avail;

  uint64_t free_ptr;
} simple_heap;

void simple_heap_init() {
  __asm__ volatile("ldr %0, =_heap_start" : "=r"(simple_heap.start));
  __asm__ volatile("ldr %0, =_heap_end" : "=r"(simple_heap.end));

  simple_heap.avail = simple_heap.end - simple_heap.start;
  simple_heap.free_ptr = simple_heap.start;

  uart_println("Simple Heap Initialized!");

  // uart_puts("Heap Starts: ");
  // uart_println(uint32_to_hex_string(simple_heap.start));
  //
  // uart_puts("Free Ptr: ");
  // uart_println(uint32_to_hex_string(simple_heap.free_ptr));
  //
  // uart_puts("Heap Ends: ");
  // uart_println(uint32_to_hex_string(simple_heap.end));

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
  if (bytes > simple_heap.avail) {
    uart_errorln("Cannot allocate. Heap full.");
    return 0;
  }

  void *addr = (void *)simple_heap.free_ptr;

  uint32_t aligned_bytes = get_32bytes_aligned_bytes(bytes);

  simple_heap.free_ptr += aligned_bytes;

  return addr;
}
