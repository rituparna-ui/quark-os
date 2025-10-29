#include "uart.h"
#include "../mmio/mmio.h"
#include "../strings/strings.h"

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
  mmio_write(UART_IBRD, 26);
  mmio_write(UART_FBRD, 3);

  // Enable FIFO & 8 bit data transmission (1 stop bit, no parity)
  mmio_write(UART_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

  // Enable UART, RX & TX
  mmio_write(UART_CR, (1 << 0) | (1 << 8) | (1 << 9));
  uart_println("UART Initialized !");
}

void uart_println(const char *str) {
  uart_puts(str);
  uart_putc('\n');
}

void uart_put_uint(uint32_t val) {
  char s[20];
  uint_to_string(val, s);
  uart_puts(s);
}

void uart_puthex(uint32_t n) {
  uart_putc('0');
  uart_putc('x');

  for (int i = 28; i >= 0; i -= 4) {
    uint32_t nibble_value = (n >> i) & 0x0F;

    char hex_char;
    if (nibble_value < 10) {
      hex_char = nibble_value + '0';
    } else {
      hex_char = (nibble_value - 10) + 'A';
    }

    uart_putc(hex_char);
  }
}
