#include "lib/uart/uart.h"
#include "lib/utils/utils.h"
#include "pmm/pmm.h"
#include <stdint.h>

void kernel_main() {
  uart_init();
  uart_println("Quark-OS Booting up...");

  print_current_el();

  pmm_init(MEM_START, MEM_SIZE);

  uintptr_t addr1 = pmm_allocate_page();
  uart_puthex(addr1);
  uart_println("");

  uintptr_t addr2 = pmm_allocate_page();
  uart_puthex(addr2);
  uart_println("");
  pmm_free_page(addr2);

  uintptr_t addr3 = pmm_allocate_page();
  uart_puthex(addr3);
  uart_println("");
  // not a double free .. addr3 got reassigned to addr2
  pmm_free_page(addr3);
  pmm_free_page(addr2); // error
  pmm_free_page(addr1);

  while (1) {
  }
}
