#include "vectors.h"
#include "../lib/uart/uart.h"

void handle() {
  uart_puts("Timer fired");
  __asm__ volatile("mrs x0, CurrentEL");
  uart_putc('s');
  return;
}
