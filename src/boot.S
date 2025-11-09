.section ".text.boot"

.global _start

_start:
  ldr x0, =_stack_top
  mov sp, x0
//  msr SP_EL0, x0


 // ldr x0, =_exception_vector_table
 // msr VBAR_EL1, x0
//

  //ldr x0, =user_space
  //msr ELR_EL1, x0
  //eret

  bl kernel_main
  # b panic
