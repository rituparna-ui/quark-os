#include "lib/uart/uart.h"
#include <stdint.h>

void kernel_main() {
  uart_init();
  uart_println("Quark-OS Booting up...");

  uint64_t val = 64;

  uart_putbin(val);
  uart_println("");
  uart_putdec(val);
  uart_println("");
  uart_puthex(val);

  while (1) {
  }
}
