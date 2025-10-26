#include "lib/mmio/mmio.h"
#include "lib/strings/strings.h"
#include "lib/uart/uart.h"
#include "lib/utils/utils.h"
#include <stdint.h>

void init_vectors();
long long c_exception_handler(long long esr, long long elr) {
  uart_puts("huehue");
  return elr + 4;
}

uint64_t c_sync_lower_handler(uint64_t esr_el1, uint64_t elr_el1) {
  uart_puts("--- Inside C Handler (EL0 -> EL1) ---\n");

  // Check Exception Class (bits 31:26)
  unsigned int ec = (unsigned int)(esr_el1 >> 26);

  // EC 0x15 means an 'svc' instruction
  if (ec == 0x15) {
    // ISS (bits 24:0) holds the immediate value
    unsigned int imm = (unsigned int)(esr_el1 & 0xFFFFFF);
    uart_puts("  Type: SVC instruction. Handled.\n");
    if (imm == 80) {
      uart_puts("  SVC number 80 received!\n");
    }
  } else {
    uart_puts("  Type: Unknown synchronous exception from EL0.\n");
  }

  // Return the *next* instruction address (svc is 4 bytes)
  return elr_el1 + 4;
}

uint64_t c_sync_current_handler(uint64_t esr_el1, uint64_t elr_el1) {
  uart_puts("--- Inside C Handler (EL1 -> EL1) ---\n");
  // (Your 'brk' handler code from before...)
  return elr_el1 + 4;
}

uint64_t c_irq_handler(uint64_t esr, uint64_t elr) {
  uart_puts("Timer fired");
  return elr + 4;
}

void user_code(void) {
  uart_puts("...Now in EL0. Triggering 'svc #80'...\n");

  // This issues a Supervisor Call.
  // The kernel's handler at 0x400 will catch it.
  __asm__ volatile("svc #0");

  uart_puts("...Returned to EL0 from svc.\n");

  while (1)
    ; // Hang
}

char user_stack[4096];

void kernel_main() {
  init_vectors();
  // 1. Set the user (EL0) stack pointer
  // __asm__ volatile("msr sp_el0, %0" : : "r"(user_stack + 4096));

  // 2. Set the EL0 return address
  // __asm__ volatile("msr elr_el1, %0" : : "r"(user_code));

  // 3. Set SPSR_EL1 to return to EL0t (M[3:0] = 0b0000)
  // __asm__ volatile("msr spsr_el1, xzr"); // xzr is the zero register

  // uart_puts("Dropping to EL0...\n");

  // 4. Return from (fake) exception to enter EL0
  // __asm__ volatile("eret");
}
