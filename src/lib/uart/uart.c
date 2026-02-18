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

void uart_puthex(uint64_t val) {
  const char hex[] = "0123456789abcdef";
  uart_puts("0x");

  for (int i = 60; i >= 0; i -= 4) {
    uart_putc(hex[(val >> i) & 0xf]);
  }
}

void uart_putdec(uint64_t val) {
  uart_puts("0d");
  if (val == 0) {
    uart_putc('0');
    return;
  }
  char buf[20];
  int i = 0;
  while (val > 0) {
    buf[i++] = '0' + (val % 10);
    val /= 10;
  }
  while (--i >= 0) {
    uart_putc(buf[i]);
  }
}

void uart_putbin(uint64_t val) {
  uart_puts("0b");

  if (val == 0) {
    uart_putc('0');
    return;
  }

  int started = 0;
  for (int i = 63; i >= 0; --i) {
    int bit = (val >> i) & 1;

    if (bit)
      started = 1;
    if (started) {
      uart_putc(bit ? '1' : '0');
    }
  }
}
