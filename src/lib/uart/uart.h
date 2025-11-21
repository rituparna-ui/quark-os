#include "../mmio/mmio.h"

#define UART_BASE 0x09000000

#define UART_DR (UART_BASE + 0x00)

void uart_putc(const char c);
void uart_puts(const char *str);
