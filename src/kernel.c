#include "lib/uart/uart.h"

void kernel_main() {
  uart_init();

  char *str = "Hello, World !!!";
  uart_println(str);

  while (1) {
    uart_putc(uart_getc());
  }
}
