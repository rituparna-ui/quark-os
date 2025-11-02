#ifndef LIB_RTC_H
#define LIB_RTC_H

#define RTC_BASE 0x09010000

#include <stdint.h>

uint32_t get_current_time();

#endif
