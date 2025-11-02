#include "gic/gic.h"
#include "lib/mmio/mmio.h"
#include "lib/rtc/rtc.h"
#include "lib/uart/uart.h"
#include "lib/utils/utils.h"
#include <stdint.h>

void kernel_main() {
  __asm__ volatile("mrs x0, CurrentEL");
  uart_init();

  print_current_el_name();

  uint32_t time = get_current_time();
  uart_puts("Current time: ");
  uart_put_uint(time);
  uart_println("");

  uart_puts("Reading rtc ris:");
  uint32_t rtcris = mmio_read(0x09010014);
  uart_put_uint(rtcris);
  uart_println("");

  uart_puts("Reading rtc imsc:");
  uint32_t rtcimsc = mmio_read(0x09010010);
  uart_put_uint(rtcimsc);
  uart_println("");
  uart_println("writing rtc imsc:");
  mmio_write(0x09010010, 1);
  uart_puts("Reading rtc imsc:");
  rtcimsc = mmio_read(0x09010010);
  uart_put_uint(rtcimsc);
  uart_println("");

  // while (1) {
  //   uint32_t time = get_current_time();
  //   uart_puts("Current time: ");
  //   uart_put_uint(time);
  //   uart_println("");
  // }

  gic_init();
  uart_println("GIC Initialized.");

  // Enable the PL031 RTC interrupt (ID 34) with a medium priority (e.g., 0xA0)
  gic_enable_interrupt(34, 0xA0);
  uart_println("RTC Interrupt Enabled in GIC.");

  uart_println("Setting a 10 second timer");
  mmio_write(0x09010004, time + 10);

  while (1) {
  }
}
