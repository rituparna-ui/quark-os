#include "lib/uart/uart.h"
#include "lib/utils/utils.h"
#include "pmm/pmm.h"
#include <stdint.h>

void kernel_main() {
  uart_init();
  uart_println("Quark-OS Booting up...");

  print_current_el();

  pmm_init(MEM_START, MEM_SIZE);

  while (1) {
  }
}
