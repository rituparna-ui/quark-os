.section ".text.vectors"

.global _exception_vector_table

_exception_vector_table:
  // KERNEL WILL NEVER USE USER SPACE STACK
  .org 0x000
  b unhandled_exception
  .org 0x080
  b unhandled_exception
  .org 0x100
  b unhandled_exception
  .org 0x180
  b unhandled_exception

  // same level exception 
  .org 0x200
  b unhandled_exception
  .org 0x280
  b unhandled_exception
  .org 0x300
  b unhandled_exception
  .org 0x380
  b unhandled_exception
  
  // lower level exception 
  .org 0x400
  b unhandled_exception
  .org 0x480
  b unhandled_exception
  .org 0x500
  b unhandled_exception
  .org 0x580
  b unhandled_exception

  // not using
  .org 0x600
  b unhandled_exception
  .org 0x680
  b unhandled_exception
  .org 0x700
  b unhandled_exception
  .org 0x780
  b unhandled_exception

unhandled_exception:
  b .
