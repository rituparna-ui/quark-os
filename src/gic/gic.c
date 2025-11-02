#include "gic.h"
#include "../lib/mmio/mmio.h" // Use the MMIO functions you already have

void gic_init_distributor() {
  // Disable the distributor before configuration
  mmio_write(GICD_CTLR, 0);

  // Read the GICD_TYPER to find out how many interrupts are supported
  uint32_t typer = mmio_read(GICD_TYPER);
  uint32_t num_interrupts = 32 * ((typer & 0x1F) + 1);

  // Disable all interrupts and clear their pending status
  for (uint32_t i = 0; i < num_interrupts / 32; ++i) {
    mmio_write(GICD_ICENABLERn(i), 0xFFFFFFFF);
    mmio_write(GICD_ICPENDRn(i), 0xFFFFFFFF);
  }

  // Set a default priority for all interrupts (e.g., 0xA0)
  // Each GICD_IPRIORITYRn register holds four 8-bit priority fields
  for (uint32_t i = 0; i < num_interrupts / 4; ++i) {
    mmio_write(GICD_IPRIORITYRn(i), 0xA0A0A0A0);
  }

  // Set all SPIs (Shared Peripheral Interrupts, ID 32+) to target CPU 0
  // Each GICD_ITARGETSRn register holds four 8-bit target fields
  for (uint32_t i = 8; i < num_interrupts / 4; ++i) {
    mmio_write(GICD_ITARGETSRn(i), 0x01010101);
  }

  // Enable the distributor (enabling both group 0 and group 1 interrupts)
  mmio_write(GICD_CTLR, 0x03);
}

void gic_init_cpu_interface() {
  // Set the priority mask register. An interrupt must have a higher
  // priority (lower value) than the value in this register to be signaled
  // to the CPU. 0xFF allows all priorities.
  mmio_write(GICC_PMR, 0xFF);

  // Enable signaling of interrupts to the CPU.
  // EnableGrp0 enables secure interrupts.
  // EnableGrp1 enables non-secure interrupts.
  mmio_write(GICC_CTLR, 0x03);
}

// Main GIC initialization function
void gic_init() {
  gic_init_distributor();
  gic_init_cpu_interface();
}

void gic_enable_interrupt(uint32_t int_id, uint8_t priority) {
  // 1. Set the priority for the interrupt
  uint32_t priority_reg_index = int_id / 4;
  uint32_t priority_shift = (int_id % 4) * 8;
  uint32_t priority_val = mmio_read(GICD_IPRIORITYRn(priority_reg_index));
  priority_val &= ~(0xFF << priority_shift);    // Clear old priority
  priority_val |= (priority << priority_shift); // Set new priority
  mmio_write(GICD_IPRIORITYRn(priority_reg_index), priority_val);

  // 2. Enable the interrupt
  uint32_t enable_reg_index = int_id / 32;
  uint32_t enable_bit = 1 << (int_id % 32);
  mmio_write(GICD_ISENABLERn(enable_reg_index), enable_bit);
}
