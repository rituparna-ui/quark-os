#include "lib/uart/uart.h"
#include "lib/utils/utils.h"

void _setup_vbar(void);

void kernel_main() {
  uart_init();

  print_current_el_name();
  uart_puts("Setting vector table");
  _setup_vbar();

  while (1) {
  }
}
