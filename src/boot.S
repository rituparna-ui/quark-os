.section ".text.boot"
.global _start

_start:
  ldr x0, =__stack_top
  mov sp, x0

  // Zero the .bss section
  // The C standard requires zero-initialized globals
  ldr x0, =__bss_start
  ldr x1, =__bss_end
  mov x2, #0
1:
  cmp x0, x1
  b.ge 2f
  str x2, [x0], #8
  b 1b
2:
  bl kernel_main
  // if kernel_main ever returns
3:
  wfe
  b 3b
