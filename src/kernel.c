#include "pci/pci.h"
#include "rng/rng.h"
#include "uart/uart.h"
#include "utils/utils.h"

#define UART_BASE 0x09000000
#define UART_DR (UART_BASE + 0x00)

void kernel_main() {
  uart_init();
  uart_println("Quark OS - Booting up !");
  print_current_el();

  pci_enumerate_bus();

  pci_virtio_rng_init();

  while (1) {
    uart_putc(uart_getc());
  }
}
