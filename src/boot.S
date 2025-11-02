.section ".text.boot"

.global _start

_start:
  ldr x0, =_stack_top
  mov sp, x0
  msr SP_EL0, x0

  ldr x0, =_exception_vector_table
  msr vbar_el1, x0

  svc 0

  b kernel_main
  # bl kernel_main
  # b panic
