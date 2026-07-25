#include "lib/uart/uart.h"
#include "lib/utils/utils.h"
#include "pmm/pmm.h"
#include <stdint.h>

void enable_fp_simd() {
  // CPACR_EL1.FPEN = 0b11 - do not trap FP/SIMD instructions
  // GCC uses SIMD registers for varargs
  // got ESR_EL1 = 0x1fe00000 while building UART printf
  uint64_t cpacr = -1;

  __asm__ __volatile__("mrs %0, cpacr_el1" : "=r"(cpacr));
  cpacr |= (3ULL << 20);
  __asm__ __volatile__("msr cpacr_el1, %0" : "=r"(cpacr));
}

void kernel_main() {
  enable_fp_simd();
  uart_init();
  uart_println("Quark-OS Booting up...");
  print_current_el();
  pmm_init(MEM_START, MEM_SIZE);

  while (1) {
  }
}
