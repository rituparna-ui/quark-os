#include "uart.h"
#include "lib/mmio/mmio.h"
#include <stdint.h>

void uart_putc(const char c) {
  mmio_write8(UART_DR, c);
  return;
}

void uart_puts(const char *str) {
  while (*str) {
    uart_putc(*str++);
  }
}
