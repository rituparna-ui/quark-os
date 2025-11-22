#include "heap/heap.h"
#include "lib/uart/uart.h"
#include "lib/utils/utils.h"

void kernel_main() {
  uart_init();
  simple_heap_init();
  uart_println("Quark OS - Booting Up");
  print_current_el();

  while (1) {
  }
}
