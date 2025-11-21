#include "uart.h"

void uart_putc(const char c) {
  mmio_write(UART_DR, c);
  return;
}

void uart_puts(const char *str) {
  while (*str) {
    mmio_write(UART_DR, *str++);
  }
}
