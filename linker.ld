ENTRY(_start)

SECTIONS
{
  .text ALIGN(4096) : {
    *(.text.boot)
    *(.text*)
  }

  .rodata ALIGN(4096) : {
    *(.rodata*)
  }

  .data ALIGN(4096) : {
    *(.data*)
  }

  .bss ALIGN(4096) (NOLOAD) : {
    __bss_start = .;
    *(.bss*)
    *(COMMON)
    __bss_end = .;
  }

  . = ALIGN(4069);
  __stack_end = .;
  . += 0x10000;      /* 64kb stack space */
  __stack_top = .;
}

