#include "lib/mmio/mmio.h"

#define UART_BASE 0x09000000
#define UART_DR (UART_BASE + 0x00)

void kernel_main() {
  char *hello = "Hello, World !";

  while (*hello) {
    mmio_write(UART_DR, *hello++);
  }

  while (1) {
  }
}
