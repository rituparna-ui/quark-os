#include "pci.h"
#include "../mmio/mmio.h"
#include "../uart/uart.h"
#include <stdint.h>

#define PCI_ECAM_BASE 0x4010000000

uint64_t pci_make_addr(uint32_t bus, uint32_t slot, uint32_t func,
                       uint32_t offset) {
  return PCI_ECAM_BASE | (((uint64_t)bus) << 20) | (((uint64_t)slot) << 15) |
         (((uint64_t)func) << 12) | ((uint64_t)(offset & 0xFFF));
}

void brute_force_scan_bus() {
  for (int bus = 0; bus < 256; bus++) {
    for (int device = 0; device < 32; device++) {
      for (int func = 0; func < 8; func++) {
        uint64_t device_address = pci_make_addr(bus, device, func, 0x00);
        uint32_t vendor_device_id = mmio_read(device_address);
        uint32_t vendor_id = vendor_device_id & 0x0000FFFF;
        if (vendor_id != 0x0000FFFF) {
          uart_puts("Device found at: ");
          uart_puthex(bus);
          uart_putc(':');
          uart_puthex(device);
          uart_putc('.');
          uart_puthex(func);
          uart_putc(' ');
          uart_puts("VendorID: ");
          uart_puthex(vendor_id);
          uart_putc('\n');
        }
      }
    }
  }
}
