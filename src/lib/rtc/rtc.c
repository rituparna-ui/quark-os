#include "rtc.h"
#include "../mmio/mmio.h"
#include <stdint.h>

uint32_t get_current_time() {
  uint32_t current_time = mmio_read(RTC_BASE);
  return current_time;
}
