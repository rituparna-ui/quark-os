#include "lib/uart/uart.h"

// Defined in gic.c
void gic_init();

// Inline assembly to enable interrupts
static void enable_interrupts() {
  asm volatile("msr daifclr, #2"); // Clear bit 2 (IRQ) of DAIF
}

void kernel_main() {
  uart_init();
  gic_init();

  uart_println("Kernel initialized. Type something to test Interrupts:");

  // Enable CPU interrupts now that everything is setup
  enable_interrupts();

  // Infinite loop doing "nothing"
  // The UART echo will happen entirely in the background via interrupts!
  while (1) {
    // asm volatile("wfi"); // Wait For Interrupt (saves power)
  }
}
