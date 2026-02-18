#include "uart.h"
#include "mmio/mmio.h"

// PL011
void uart_init() {
  // disable uart
  mmio_write(UART_CR, 0x00000000);

  // clear pending interrupts
  mmio_write(UART_ICR, 0x7FF);

  // setup baudrate
  // Divider = UART_CLOCK/(16 * Baud)
  // UART_CLOCK = 48000000; Baud = 115200
  // Divider = 48000000/(16 * 115200) = 26.041666667
  // Integer = 26
  // Fraction = 0.041667
  // Fraction register = round(0.0416667 * 2^6) = 3
  // FBRD is a 6 bit number
  mmio_write(UART_IBRD, 26);
  mmio_write(UART_FBRD, 3);

  // Enable FIFO & 8 bit data transmission (1 stop bit, no parity)
  mmio_write(UART_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

  // Enable UART, RX & TX
  mmio_write(UART_CR, (1 << 0) | (1 << 8) | (1 << 9));
  uart_println("UART Initialized !");
}

void uart_putc(const char c) {
  // check if transmit fifo is full - TXFF
  while (mmio_read(UART_FR) & (1 << 5)) {
  }
  mmio_write(UART_DR, c);
}

void uart_puts(const char *str) {
  while (*str) {
    uart_putc(*str++);
  }
}

unsigned char uart_getc(void) {
  // check if receive fifo is empty - RXFE
  while (mmio_read(UART_FR) & (1 << 4)) {
  }
  return mmio_read(UART_DR);
}

void uart_println(const char *str) {
  uart_puts(str);
  uart_putc('\n');
}
