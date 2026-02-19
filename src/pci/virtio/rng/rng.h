#ifndef VIRTIO_RNG
#define VIRTIO_RNG

#include "pci/pci.h"

#define VIRTIO_RNG_VENDOR_ID 0x1AF4
#define VIRTIO_RNG_DEVICE_ID 0x1044

struct virtio_rng {
  struct pci_device pci;
};

void pci_virtio_rng_init(void);

#endif // !VIRTIO_RNG
