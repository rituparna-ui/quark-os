#include "lib/uart/uart.h"
#include <stdint.h>

void kernel_main() {
  char *str = "Quark-OS Booting up...";

  uart_puts(str);

  while (1) {
  }
}
