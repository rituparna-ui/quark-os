#ifndef PCI_H
#define PCI_H

#define MAX_PCI_BUS 256
#define MAX_PCI_SLOT 32
#define MAX_PCI_FUNC 8

#define PCI_ECAM_BASE 0x4010000000UL

/* PCI config space offsets */
#define PCI_VENDOR_ID 0x00
#define PCI_DEVICE_ID 0x02

void pci_enumerate_bus(void);

#endif // !PCI_H
