ENTRY(_start)

SECTIONS {
  . = 0x40000000; /* 0x40100000, 0x41000000 */

  .text : {
    *(.text.boot)
    *(.text)
  }

  .data : {
    *(.data)
  }

  .bss : {
    *(.bss)
    *(COMMON)
  }

  _stack_end = .;
  . += 0x100000; /* 1mB of stack */
  _stack_top = .;

  _heap_start = .;
  . += 0x20000000; /* 512mB of heap */
  _heap_end = .;
}
