#include "pci/pci.h"
#include "mmio/mmio.h"
#include "uart/uart.h"

struct pci_device pci_devices[MAX_PCI_DEVICES];
uint16_t pci_device_count = 0;

uintptr_t pci_make_ecam_addr(uint8_t bus, uint8_t slot, uint8_t func,
                             uint8_t offset) {
  return PCI_ECAM_BASE | ((uintptr_t)bus << 20) | ((uintptr_t)slot << 15) |
         ((uintptr_t)func << 12) | (uintptr_t)offset;
}

uint16_t pci_config_read16(uint8_t bus, uint8_t slot, uint8_t func,
                           uint16_t offset) {
  return mmio_read16(pci_make_ecam_addr(bus, slot, func, offset));
}

void pci_enumerate_bus(void) {
  uart_println("[PCI] Enumerating PCI devices");

  for (uint16_t bus = 0; bus < MAX_PCI_BUS; bus++) {
    for (uint8_t slot = 0; slot < MAX_PCI_SLOT; slot++) {
      for (uint8_t func = 0; func < MAX_PCI_FUNC; func++) {
        uint16_t vendor_id = pci_config_read16(bus, slot, func, PCI_VENDOR_ID);
        if (vendor_id == 0xFFFF) {
          continue;
        }
        uint16_t device_id = pci_config_read16(bus, slot, func, PCI_DEVICE_ID);
        uart_puts("[PCI] Device found at ");
        uart_putdec(bus);
        uart_putc(':');
        uart_putdec(slot);
        uart_putc(':');
        uart_putdec(func);
        uart_puts(" | VendorID: ");
        uart_puthex(vendor_id);
        uart_puts(", DeviceID: ");
        uart_puthex(device_id);
        uart_println("");

        if (pci_device_count >= MAX_PCI_DEVICES) {
          uart_errorln("[PCI] Max PCI devices limit reached");
          return;
        }

        pci_devices[pci_device_count] = (struct pci_device){
            .bus = bus,
            .slot = slot,
            .func = func,
            .vendor_id = vendor_id,
            .device_id = device_id,
        };
        pci_device_count++;
      }
    }
  }
}
