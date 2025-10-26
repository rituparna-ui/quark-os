#include "lib/uart/uart.h"
#include "lib/utils/utils.h"

void kernel_main() {
  uart_init();

  print_current_el_name();

  while (1) {
  }
}
