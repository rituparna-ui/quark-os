#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define MEM_START 0x40000000
#define MEM_SIZE (8ULL * 1024 * 1024 * 1024)
#define PAGE_SIZE 4096

// Align address up/down to page boundary
// clear lower 12 bits
// addr = 0x12345 | up = 0x13000 | down = 0x12000
#define PAGE_ALIGN_UP(addr) (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ALIGN_DOWN(addr) ((addr) & ~(PAGE_SIZE - 1))

// each uint64_t holds 64 page bits
#define BITMAP_INDEX(pfn) ((pfn) / 64)
#define BITMAP_BIT(pfn) ((pfn) % 64)

// Convert between page frame number and physical address
// phys_addr = pfn * page_size
// x << 12 == x * 4096
// works for page aligned addresses
#define PAGE_SHIFT 12
// Page size = 2^PAGE_SHIFT
#define PFN_TO_PHYS(pfn) ((uint64_t)(pfn) << PAGE_SHIFT)
#define PHYS_TO_PFN(addr) ((uint64_t)(addr) >> PAGE_SHIFT)

void pmm_init(uintptr_t mem_start, uint64_t mem_size);
uintptr_t pmm_allocate_page(void);
void pmm_free_page(uintptr_t phys_addr);

#endif // !PMM_H
