#ifndef PCI_H
#define PCI_H

#include <stdint.h>

#define MAX_PCI_BUS 256
#define MAX_PCI_SLOT 32
#define MAX_PCI_FUNC 8

#define MAX_PCI_DEVICES 8

#define PCI_ECAM_BASE 0x4010000000UL

/* PCI config space offsets */
#define PCI_VENDOR_ID 0x00
#define PCI_DEVICE_ID 0x02
#define PCI_COMMAND 0x04
#define PCI_STATUS 0x06
#define PCI_REV_ID 0x08
#define PCI_CLASS_CODE 0x09
#define PCI_HEADER_TYPE 0x0E
#define PCI_BAR_0 0x10
#define PCI_CAPABILITY_LIST 0x34

struct pci_device {
  uint8_t bus;
  uint8_t slot;
  uint8_t func;

  uint16_t vendor_id;
  uint16_t device_id;
};

void pci_enumerate_bus(void);

#endif // !PCI_H
