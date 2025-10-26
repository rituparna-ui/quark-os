.section .text.vectors
.align 11

.global _exception_vector_table

_exception_vector_table:
  // Group 1: 
  .org 0x000
  b unhandled_exception
  .org 0x080
  b unhandled_exception
  .org 0x100
  b unhandled_exception
  .org 0x180
  b unhandled_exception

  // Group 2: 
  .org 0x200
  b sync_current_elx_handler
  .org 0x280
  b irq_handler
  .org 0x300
  b unhandled_exception
  .org 0x380
  b unhandled_exception
  
  // Group 3: 
  .org 0x400
  b sync_lower_el_aarch64_handler
  .org 0x480
  b irq_handler
  .org 0x500
  b unhandled_exception
  .org 0x580
  b unhandled_exception
  
  // Group 4: 
  .org 0x600
  b unhandled_exception
  .org 0x680
  b unhandled_exception
  .org 0x700
  b unhandled_exception
  .org 0x780
  b unhandled_exception

.section .text

.global init_vectors

init_vectors:
  ldr x0, =_exception_vector_table
  msr vbar_el1, x0
  isb
  ret

.global irq_handler
irq_handler:
  stp     x0, x1, [sp, #-16]!
  stp     x29, x30, [sp, #-16]!
  mov     x29, sp
  mrs     x0, esr_el1             // Arg 1: Exception Syndrome Register
  mrs     x1, elr_el1             // Arg 2: Exception Link Register (return address)
  bl      c_irq_handler

  // 3. The C handler returns the new, correct return address in x0.
  //    We must set ELR_EL1 to this new address.
  msr     elr_el1, x0

  // 4. Restore saved registers
  mov     sp, x29
  ldp     x29, x30, [sp], #16
  ldp     x0, x1, [sp], #16

  // 5. Return from the exception
  eret

sync_current_elx_handler:
  stp     x0, x1, [sp, #-16]!
  stp     x29, x30, [sp, #-16]!
  mov     x29, sp
  mrs     x0, esr_el1             // Arg 1: Exception Syndrome Register
  mrs     x1, elr_el1             // Arg 2: Exception Link Register (return address)
  bl      c_exception_handler

  // 3. The C handler returns the new, correct return address in x0.
  //    We must set ELR_EL1 to this new address.
  msr     elr_el1, x0

  // 4. Restore saved registers
  mov     sp, x29
  ldp     x29, x30, [sp], #16
  ldp     x0, x1, [sp], #16

  // 5. Return from the exception
  eret

.globl sync_lower_el_aarch64_handler
sync_lower_el_aarch64_handler:
    // 1. Save registers
    stp     x0, x1, [sp, #-16]!
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp

    // 2. Read exception info and call C handler
    mrs     x0, esr_el1             // Arg 1: Syndrome Register
    mrs     x1, elr_el1             // Arg 2: Return Address
    bl      c_sync_lower_handler

    // 3. Set the new return address (from C)
    msr     elr_el1, x0

    // 4. Restore registers and return
    mov     sp, x29
    ldp     x29, x30, [sp], #16
    ldp     x0, x1, [sp], #16
    
    eret

unhandled_exception:
  b .
