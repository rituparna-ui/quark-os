#ifndef LIB_MMIO_H
#define LIB_MMIO_H

#include <stdint.h>

void mmio_write(uintptr_t addr, uint32_t value);
uint32_t mmio_read(uintptr_t addr);

// GICv2 Addresses for QEMU virt
#define GIC_DIST_BASE 0x08000000
#define GIC_CPU_BASE 0x08010000

#define GICD_CTLR (GIC_DIST_BASE + 0x000)
#define GICD_ISENABLER (GIC_DIST_BASE + 0x100)
#define GICD_ITARGETSR (GIC_DIST_BASE + 0x800)

#define GICC_CTLR (GIC_CPU_BASE + 0x000)
#define GICC_PMR (GIC_CPU_BASE + 0x004)
#define GICC_IAR (GIC_CPU_BASE + 0x00C)
#define GICC_EOIR (GIC_CPU_BASE + 0x010)

#endif // !LIB_MMIO_H
