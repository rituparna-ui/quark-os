#include "mmio.h"
#include <stdint.h>

void mmio_write(uintptr_t addr, uint32_t value) {
  *(volatile uint32_t *)addr = value;
  return;
}

uint32_t mmio_read(uintptr_t addr) {
  uint32_t value = *(volatile uint32_t *)addr;
  return value;
}
