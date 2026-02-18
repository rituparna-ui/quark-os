#include "uart.h"

void uart_putc(const char c) { mmio_write(UART_DR, c); }

void uart_puts(const char *str) {
  while (*str) {
    uart_putc(*str++);
  }
}
