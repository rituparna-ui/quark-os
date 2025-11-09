#include "lib/uart/uart.h"
#include "lib/utils/utils.h"
#include <stdint.h>

#define PCI_ECAM_BASE 0x10000000

void user_space() {
  uart_println("entered in user space");
  __asm__ volatile("nop");

  print_current_el_name();
  while (1) {
  }
}

void kernel_main() {
  uart_init();

  print_current_el_name();

  __asm__ volatile("svc 0");
  // __asm__ volatile("msr ");

  return;
}
