#ifndef HEAP_H
#define HEAP_H

#include "../lib/uart/uart.h"

#define DIM_M 32
#define DIM_L 64
#define DIM_K 64
#define DIM_J 64
#define DIM_I 64

#define HEAP_MAP_SIZE 36

#define ALIGNMENT 32

void heap_init(void);
void *qalloc(uint32_t num_bytes);
void qfree(void *addr);

#endif // !HEAP_H
