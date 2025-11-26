#include "lib/mmio/mmio.h"
#include "lib/uart/uart.h"

// UART0 interrupt on QEMU virt is SPI 1.
// SPI start at 32. So 32 + 1 = 33.
#define UART_IRQ_ID 33

void gic_init() {
  // 1. Distributor Setup
  mmio_write(GICD_CTLR, 0); // Disable distributor during setup

  // Enable UART Interrupt (ID 33)
  // Register calculates as: Base + 0x100 + (33 / 32) * 4
  uintptr_t enable_reg = GICD_ISENABLER + (UART_IRQ_ID / 32) * 4;
  mmio_write(enable_reg, (1 << (UART_IRQ_ID % 32)));

  // Route interrupt to CPU 0
  // Each target field is 8 bits. Register holds 4 targets.
  uintptr_t target_reg = GICD_ITARGETSR + (UART_IRQ_ID / 4) * 4;
  uint32_t current_targets = mmio_read(target_reg);
  // Add CPU 0 (bit 1) to the target byte for our ID
  current_targets |= (1 << ((UART_IRQ_ID % 4) * 8));
  mmio_write(target_reg, current_targets);

  mmio_write(GICD_CTLR, 1); // Enable distributor

  // 2. CPU Interface Setup
  mmio_write(GICC_PMR, 0xFF); // Priority Mask: Accept all priorities
  mmio_write(GICC_CTLR, 1);   // Enable CPU interface
}

// Called from assembly
void handle_irq() {
  // Acknowledge Interrupt (Read IAR)
  uint32_t irq_ack = mmio_read(GICC_IAR);
  uint32_t irq_id = irq_ack & 0x3FF;

  if (irq_id == UART_IRQ_ID) {
    uart_handle_irq();
  }

  // End of Interrupt (Write EOIR)
  mmio_write(GICC_EOIR, irq_ack);
}
