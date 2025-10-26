#ifndef MMIO_LIB_H
#define MMIO_LIB_H

#include <stdint.h>

void mmio_write(uintptr_t addr, uint32_t value);
uint32_t mmio_read(uintptr_t addr);

#endif
