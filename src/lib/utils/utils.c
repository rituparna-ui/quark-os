#include "utils.h"

uint32_t get_current_el_number() {
  uint32_t el = -1;

  // Move to Register from System Register CurrentEL
  asm volatile("mrs %0, CurrentEL" : "=r"(el));

  // EL is stored in bits 2 and 3
  return (el >> 2) & 0b11;
}

char *get_el_name(uint32_t el) {
  switch (el) {
  case 0:
    return "User Space";
  case 1:
    return "Kernel Space";
  case 2:
    return "Hyper Space";
  case 3:
    return "Secure Monitor";
  default:
    return "Invalid Exception Level";
  }
}

void print_current_el() {
  uint32_t el = get_current_el_number();
  char *el_name = get_el_name(el);
  uart_puts("Current Exception Level: ");
  uart_println(el_name);
  return;
}
