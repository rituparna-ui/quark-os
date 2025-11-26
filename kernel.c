#include <stdint.h>

/* * UART Address:
 * In High Map (TTBR1), we mapped the first 1GB of physical RAM
 * to the base of the kernel space.
 * * High Base: 0xFFFFFF80 00000000
 * UART Phys: 0x00000000 09000000
 * UART Virt: 0xFFFFFF80 09000000
 */
#define UART0_DR ((volatile unsigned int *)(0xFFFFFF8009000000))

void uart_putc(char c) { *UART0_DR = c; }

void uart_puts(const char *s) {
  while (*s) {
    uart_putc(*s++);
  }
}

void print_hex(uint64_t n) {
  const char *hexdigits = "0123456789ABCDEF";
  for (int i = 60; i >= 0; i -= 4) {
    uart_putc(hexdigits[(n >> i) & 0xF]);
  }
}

void kernel_main() {
  uart_puts("MMU Enabled.\n");
  uart_puts("Kernel is running in Higher Half!\n");

  uint64_t pc;
  asm volatile("adr %0, ." : "=r"(pc));

  uart_puts("Current PC: 0x");
  print_hex(pc);
  uart_puts("\n");

  /* * If PC starts with FFFFFF80..., you succeeded.
   * If PC starts with 00000000... or 40000000..., the jump failed.
   */

  while (1) {
  }
}
