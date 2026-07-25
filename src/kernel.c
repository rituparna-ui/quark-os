#include <stdint.h>

#define UART_BASE 0x9000000UL
#define UART_DR (UART_BASE + 0x00)

void kernel_main() {
  char *str = "Hello, World !";

  while (*str) {
    *(volatile uint32_t *)UART_BASE = *str++;
  }

  while (1) {
  }
}
