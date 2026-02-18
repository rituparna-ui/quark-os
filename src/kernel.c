#include "lib/uart/uart.h"

#define UART_BASE 0x09000000
#define UART_DR (UART_BASE + 0x00)

void kernel_main() {
  char *hello = "Hello, World !\n";

  uart_puts(hello);
  uart_puts("Quark OS - v0");

  while (1) {
  }
}
