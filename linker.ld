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

  . += 0x1000; /* 4kB of stack */

  _stack_top = .;
}
