.section .text.vectors

.global _excption_vector_table
.global _setup_vbar
.align 11

_excption_vector_table:
  // Kernel will never use user space stack
  .org 0x000
  bl unhandled_exception
  .org 0x080
  bl unhandled_exception
  .org 0x100
  bl unhandled_exception
  .org 0x180
  bl unhandled_exception
  
  // EL1 -> EL1
  .org 0x200
  bl unhandled_exception
  .org 0x280
  bl unhandled_exception
  .org 0x300
  bl unhandled_exception
  .org 0x380
  bl unhandled_exception
  
  // EL0 -> EL1
  .org 0x400
  bl unhandled_exception
  .org 0x480
  bl unhandled_exception
  .org 0x500
  bl unhandled_exception
  .org 0x580
  bl unhandled_exception
  
  .org 0x600
  bl unhandled_exception
  .org 0x680
  bl unhandled_exception
  .org 0x700
  bl unhandled_exception
  .org 0x780
  bl unhandled_exception

unhandled_exception:
  b .

_setup_vbar:
  ldr x0, =_excption_vector_table
  msr VBAR_EL1, x0
  ret
