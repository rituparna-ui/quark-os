#ifndef GIC_H
#define GIC_H

#include <stdint.h>

#define GICD_BASE 0x08000000
#define GICC_BASE 0x08010000

/* GIC Distributor Register Offsets */
#define GICD_CTLR (GICD_BASE + 0x000)  // Distributor Control Register
#define GICD_TYPER (GICD_BASE + 0x004) // Interrupt Controller Type Register
#define GICD_ISENABLERn(n)                                                     \
  (GICD_BASE + 0x100 + (n) * 4) // Interrupt Set-Enable Registers
#define GICD_ICENABLERn(n)                                                     \
  (GICD_BASE + 0x180 + (n) * 4) // Interrupt Clear-Enable Registers
#define GICD_ICPENDRn(n)                                                       \
  (GICD_BASE + 0x280 + (n) * 4) // Interrupt Clear-Pending Registers
#define GICD_IPRIORITYRn(n)                                                    \
  (GICD_BASE + 0x400 + (n) * 4) // Interrupt Priority Registers
#define GICD_ITARGETSRn(n)                                                     \
  (GICD_BASE + 0x800 + (n) * 4) // Interrupt Processor Targets Registers

/* GIC CPU Interface Register Offsets */
#define GICC_CTLR (GICC_BASE + 0x00) // CPU Interface Control Register
#define GICC_PMR (GICC_BASE + 0x04)  // Interrupt Priority Mask Register
#define GICC_IAR (GICC_BASE + 0x0C)  // Interrupt Acknowledge Register
#define GICC_EOIR (GICC_BASE + 0x10) // End of Interrupt Register

void gic_init(void);
void gic_enable_interrupt(uint32_t int_id, uint8_t priority);

#endif // GIC_H
