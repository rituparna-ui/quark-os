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
  . += 0x4000; /* 16kB of stack */
  _stack_top = .;
}
