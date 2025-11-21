#include "mmio.h"
#include <stdint.h>

void mmio_write32(uintptr_t addr, uint32_t value) {
  *(volatile uint32_t *)addr = value;
  return;
}

uint32_t mmio_read32(uintptr_t addr) {
  uint32_t value = *(volatile uint32_t *)addr;
  return value;
}

uint32_t mmio_read(uintptr_t addr) { return mmio_read32(addr); }

void mmio_write(uintptr_t addr, uint32_t value) {
  return mmio_write32(addr, value);
}
