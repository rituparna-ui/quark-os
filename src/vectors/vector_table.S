.section ".text.vectors"
.align 11

.global _exception_vector_table

_exception_vector_table:
  .org 0x000
  bl handle
  eret
  .org 0x080
  bl handle
  eret
  .org 0x100
  bl handle
  eret
  .org 0x180
  bl handle
  eret

  .org 0x200
  bl handle
  eret
  .org 0x280
  bl handle
  eret
  .org 0x300
  bl handle
  eret
  .org 0x380
  bl handle
  eret

  .org 0x400
  bl handle
  eret
  .org 0x480
  bl handle
  eret
  .org 0x500
  bl handle
  eret
  .org 0x580
  bl handle
  eret

  .org 0x600
  bl handle
  eret
  .org 0x680
  bl handle
  eret
  .org 0x700
  bl handle
  eret
  .org 0x780
  bl handle
  eret

