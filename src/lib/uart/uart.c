#include "uart.h"

void uart_putc(const char c) {
  // check if transmit fifo is full - TXFF
  while (mmio_read(UART_FR) & (1 << 5)) {
  }
  mmio_write(UART_DR, c);
}

void uart_puts(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    uart_putc(str[i]);
  }
}

unsigned char uart_getc(void) {
  // check if receive fifo is empty - RXFE
  while (mmio_read(UART_FR) & (1 << 4)) {
  }
  return mmio_read(UART_DR);
}

void uart_init() {
  mmio_write(UART_CR, 0x00000000);
  mmio_write(UART_ICR, 0x7FF);

  // Baud rate setup
  mmio_write(UART_IBRD, 26);
  mmio_write(UART_FBRD, 3);

  mmio_write(UART_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

  // Enable RX Interrupt (Bit 4).
  // We mask it (set it to 1) to enable passing to the interrupt controller.
  mmio_write(UART_IMSC, (1 << 4));

  mmio_write(UART_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_println(const char *str) {
  uart_puts(str);
  uart_putc('\n');
}

void uart_handle_irq(void) {
  // Check if it is a Receive Interrupt (RXMIS)
  if (mmio_read(UART_MIS) & (1 << 4)) {
    char c = (char)mmio_read(UART_DR);

    // ECHO: Print it back immediately to prove it works
    // In a real OS, you would write this to a Ring Buffer
    uart_putc(':');
    uart_putc(c);

    // Clear the interrupt
    mmio_write(UART_ICR, (1 << 4));
  }
}
