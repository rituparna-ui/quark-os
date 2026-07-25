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
  . = . + 0x10000; /* 64 KiB kernel stack */
  __stack_top = .; /* stack grows downwards */

  . = ALIGN(4096);
  __kernel_end = .; /* Everything after this is free for PMM */
}
