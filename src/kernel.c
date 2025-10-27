#include "lib/uart/uart.h"
#include "lib/utils/utils.h"

void kernel_main() {
  uart_init();

  print_current_el_name();

  uart_put_uint(81283924);

  while (1) {
  }
}
