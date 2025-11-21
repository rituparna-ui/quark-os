#ifndef LIB_MMIO_H
#define LIB_MMIO_H

#include <stdint.h>

void mmio_write(uintptr_t addr, uint32_t value);
uint32_t mmio_read(uintptr_t addr);

void mmio_write32(uintptr_t addr, uint32_t value);
uint32_t mmio_read32(uintptr_t addr);

#endif // !LIB_MMIO_H
