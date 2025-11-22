#include "heap/heap.h"
#include "lib/uart/uart.h"

void kernel_main() {
  uart_init();
  simple_heap_init();

  while (1) {
  }
}
