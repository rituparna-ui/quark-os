ENTRY(_start)

SECTIONS
{
  . = 0x40000000;

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

  .bss ALIGN(4096) : {
    *(.bss*)
    *(COMMON)
  }

  . = ALIGN(4096);
  __stack_top = . + 0x10000;
}
