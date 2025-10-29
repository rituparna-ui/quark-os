#include "lib/pci/pci.h"
#include "lib/uart/uart.h"
#include "lib/utils/utils.h"

void kernel_main() {
  uart_init();
  print_current_el_name();
  brute_force_scan_bus();

  while (1) {
  }
}
