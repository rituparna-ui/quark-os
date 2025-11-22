#ifndef HEAP_H
#define HEAP_H

#include "../lib/strings/strings.h"
#include "../lib/uart/uart.h"
#include <stdint.h>

void simple_heap_init(void);
void *simple_heap_alloc(uint32_t bytes);

#endif // !HEAP_H
