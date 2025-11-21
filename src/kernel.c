#include "lib/uart/uart.h"

void kernel_main() {
  char *str = "Hello, World !!!";
  uart_puts(str);

  while (1) {
  }
}
