#include "lib/uart/uart.h"
#include "lib/utils/utils.h"

void init_vectors(void);

long long c_exception_handler(long long esr_el1, long long elr_el1) {
  uart_puts("--- Inside C Exception Handler ---\n");

  // Check the Exception Class (bits 31:26)
  unsigned int ec = (unsigned int)(esr_el1 >> 26);

  // EC 0x3C means a 'brk' instruction
  if (ec == 0x3C) {
    uart_puts("  Type: BRK instruction. Handled.\n");
  } else {
    uart_puts("  Type: Unhandled synchronous exception.\n");
  }

  // We *must* return the address of the *next* instruction.
  // If we return 'elr_el1', we will re-execute the 'brk'
  // and be stuck in an infinite exception loop.
  // A 'brk' instruction is 4 bytes long.
  return elr_el1 + 4;
}

void kernel_main() {
  uart_init();

  print_current_el_name();

  init_vectors();
  uart_puts("Vector table initialized.\n");

  // 2. Trigger a synchronous exception
  uart_puts("Triggering 'brk #0' exception...\n");

  __asm__ volatile("brk #0");

  // 3. If the handler works, we will return here
  uart_puts("...returned from exception successfully.\n");

  // 4. Trigger another one to prove it wasn't a fluke
  uart_puts("Triggering 'brk #1' exception...\n");

  __asm__ volatile("brk #1");

  uart_puts("...returned from exception successfully.\n");

  uart_puts("Test complete. Halting.\n");

  while (1) {
  }
}
