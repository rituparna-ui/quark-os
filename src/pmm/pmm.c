#include "pmm.h"
#include "lib/strings/strings.h" // IWYU pragma: keep
#include "lib/uart/uart.h"
#include <stdint.h>

extern uintptr_t __kernel_end;

static uint64_t *bitmap;
static uint64_t bitmap_size;
static uint64_t total_pages;
static uint64_t used_pages;
static uint64_t reserved_pages;

static uint64_t mem_region_start;
static uint64_t mem_region_end;

static inline void bitmap_set(uint64_t pfn) {
  bitmap[BITMAP_INDEX(pfn)] |= (1ULL << BITMAP_BIT(pfn));
}

static inline void bitmap_clear(uint64_t pfn) {
  bitmap[BITMAP_INDEX(pfn)] &= ~(1ULL << BITMAP_BIT(pfn));
}

static inline int bitmap_test(uint64_t pfn) {
  return (bitmap[BITMAP_INDEX(pfn)] >> BITMAP_BIT(pfn)) & 1;
}

void pmm_print_info(void) {
  uart_printf("[PMM][INFO] Memory Region: %x - %x\n", mem_region_start,
              mem_region_end);
  uart_printf("[PMM][INFO] Memory Size: %x bytes | %d mbytes\n", MEM_SIZE,
              MEM_SIZE / 1024 / 1024);
  uart_printf("[PMM][INFO] Total Pages: %u\n", total_pages);
  uart_printf("[PMM][INFO] Reserved Pages: %u\n", reserved_pages);
  uart_printf("[PMM][INFO] Used Pages: %u\n", used_pages);
  uart_printf("[PMM][INFO] Free Pages: %u\n", total_pages - used_pages);
}

void pmm_init(uintptr_t mem_start, uint64_t mem_size) {
  uart_println("[PMM] Initializing Physical Memory Manager");

  mem_region_start = mem_start;
  mem_region_end = mem_start + mem_size;

  total_pages = mem_size / PAGE_SIZE;

  // BITMAP
  // one bit per page
  // ceil(total_pages / 64) uint64_t entries
  bitmap_size = (total_pages + 63) / 64;
  uint64_t bitmap_bytes = bitmap_size * sizeof(uint64_t);

  // Place bitmap at the first page aligned address after the end of kernel code
  uint64_t kernel_end = (uint64_t)&__kernel_end;
  bitmap = (uint64_t *)PAGE_ALIGN_UP(kernel_end);

  // kernel image + stack + bitmap
  uint64_t bitmap_end = (uint64_t)bitmap + bitmap_bytes;
  uint64_t reserved_end = PAGE_ALIGN_UP(bitmap_end);
  reserved_pages = (reserved_end - mem_region_start) / PAGE_SIZE;

  // zero the bitmap before marking reserved pages
  memset(bitmap, 0, bitmap_bytes);

  // Mark reserved pages
  for (uint64_t pfn = 0; pfn < reserved_pages; pfn++) {
    bitmap_set(pfn);
  }

  used_pages = reserved_pages;
  uart_println("[PMM] Initialized !");
  pmm_print_info();
}

uintptr_t pmm_allocate_page(void) {
  uart_puts("[PMM] allocating 1 page at: ");

  for (uint64_t i = 0; i < bitmap_size; i++) {
    if (bitmap[i] == ~0ULL) {
      // skip all ones
      continue;
    }

    // find first unset bit in the current uint64_t entry
    for (uint8_t bit = 0; bit < 64; bit++) {
      uint64_t page_frame_number = i * 64 + bit;

      if (page_frame_number >= total_pages) {
        uart_errorln("[PMM] Out of range pfm.");
        return 0;
      }

      if (!bitmap_test(page_frame_number)) {
        bitmap_set(page_frame_number);
        used_pages++;
        uintptr_t phys_addr = mem_region_start + PFN_TO_PHYS(page_frame_number);
        uart_puthex(phys_addr);
        uart_println("phys addr<-");
        return phys_addr;
      }
    }
  }

  uart_errorln("[PMM] Out of memory! No free pages available.");
  return 0;
}

void pmm_free_page(uintptr_t phys_addr) {
  uart_puts("[PMM] attempting to free page/address: ");
  uart_puthex(phys_addr);
  uart_println("");

  if (phys_addr < mem_region_start || phys_addr >= mem_region_end) {
    uart_errorln("[PMM] address outside managed region");
    return;
  }

  if (phys_addr & (PAGE_SIZE - 1)) {
    // lower 12 bits are non zero
    uart_errorln("[PMM] non page aligned address");
    return;
  }

  uint64_t page_frame_number = PHYS_TO_PFN(phys_addr - mem_region_start);

  if (page_frame_number < reserved_pages) {
    uart_errorln("[PMM] reserved page");
    return;
  }

  if (!bitmap_test(page_frame_number)) {
    uart_errorln("[PMM] unallocated page");
    return;
  }

  bitmap_clear(page_frame_number);
  used_pages--;
}
