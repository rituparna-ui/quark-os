#include "uart/uart.h"
#include "utils/utils.h"

#define UART_BASE 0x09000000
#define UART_DR (UART_BASE + 0x00)

void kernel_main() {
  uart_init();
  uart_println("Quark OS - Booting up !");
  print_current_el();

  while (1) {
    uart_putc(uart_getc());
  }
}
