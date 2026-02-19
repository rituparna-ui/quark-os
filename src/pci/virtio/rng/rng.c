#include "rng/rng.h"
#include "pci/pci.h"
#include "uart/uart.h"

struct virtio_rng rng_dev;

void pci_virtio_rng_init() {
  uart_println("[VIRTIO-RNG] Initializing Device");

  /* Step 0 */
  if (!pci_find_device(VIRTIO_RNG_VENDOR_ID, VIRTIO_RNG_DEVICE_ID,
                       &rng_dev.pci)) {
    uart_errorln("[VIRTIO-RNG] Device not found");
    return;
  }
  uart_println("[VIRTIO-RNG] Device found");

  uint8_t header_type = pci_get_header_type(&rng_dev.pci);
  if (header_type != PCI_NORMAL_DEV_HEADER_TYPE) {
    uart_errorln("[VIRTIO-RNG] Unexpected header type");
    return;
  }

  // assign bars
  pci_assign_bars(&rng_dev.pci);
  // enable device
}
