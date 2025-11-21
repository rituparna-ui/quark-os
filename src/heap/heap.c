#include "heap.h"
#include <stdint.h>

// extern uint8_t _heap_start;
// extern uint8_t _heap_end;
// (uint32_t)(&_heap_start) can also be used

// total heap size is 512mb (0x20000000)
// size of each block = 32bytes
// total number of blocks = 512mb / 32 bytes = 64^4
// can be represented as an array
// uint64 [64^3]
struct QHeap {
  uint64_t start;
  uint64_t end;

  uint64_t allocated;

  uint64_t free_ptr;
} heap;

uint64_t busy_map[64 * 64 * 64] = {0};
uint64_t head_map[64 * 64 * 64] = {0};

void heap_init() {
  __asm__ volatile("ldr %0, =_heap_start" : "=r"(heap.start));
  __asm__ volatile("ldr %0, =_heap_end" : "=r"(heap.end));

  heap.free_ptr = heap.start;

  uart_println("Heap Initialized !");
}

uint32_t get_32byte_aligned_size(uint32_t num_bytes) {
  if (num_bytes == 0) {
    return 0;
  }
  uint32_t num_blocks = (num_bytes + ALIGNMENT - 1) / ALIGNMENT;

  return num_blocks * ALIGNMENT;
}

void *qalloc(uint32_t num_bytes) {
  if (heap.allocated + num_bytes > 0x20000000) {
    uart_errorln("Unable to allocate memory. Not enough space in the heap.");
    return 0;
  }

  // if (heap.free_ptr + num_bytes > 0x20000000) {
  //   //
  // }

  uint32_t aligned_num_byte = get_32byte_aligned_size(num_bytes);
  uint32_t num_blocks = aligned_num_byte / ALIGNMENT;

  find_free_blocks(num_blocks);

  return 0;
}

void qfree(void *addr) {}
