.section .text.vectors
.align 11

.global _exception_vector_table

_exception_vector_table:
  // Group 1: Exceptions from Current EL, using SP_EL0 - Not Using !!!
  .org 0x000
  b unhandled_exception
  .org 0x080
  b unhandled_exception
  .org 0x100
  b unhandled_exception
  .org 0x180
  b unhandled_exception

  // Group 2: Exceptions from Current EL, using SP_ELx
  .org 0x200
  b sync_current_elx_handler
  .org 0x280
  b unhandled_exception
  .org 0x300
  b unhandled_exception
  .org 0x380
  b unhandled_exception

  // Group 3: Exceptions from Lower EL (EL0), AArch64 - System Calls - svc
  .org 0x400
  b unhandled_exception
  .org 0x480
  b unhandled_exception
  .org 0x500
  b unhandled_exception
  .org 0x580
  b unhandled_exception

  // Group 4: Exceptions from Lower EL, AArch32
  .org 0x600
  b unhandled_exception
  .org 0x680
  b unhandled_exception
  .org 0x700
  b unhandled_exception
  .org 0x780
  b unhandled_exception

.section .text

/* This is the C-callable function to set VBAR_EL1 */
.globl init_vectors
init_vectors:
    ldr     x0, =_exception_vector_table  // 1. Load the address of our vector table
    msr     vbar_el1, x0            // 2. Set VBAR_EL1 to this address
    isb                             // 3. Instruction barrier to ensure it's set
    ret                             // 4. Return to C code

/* * This is our actual handler for synchronous exceptions at EL1.
 * It will be called when we execute the 'brk' instruction from C.
 */
sync_current_elx_handler:
    // 1. Save the registers we are about to clobber (x0, x1) and the link register (x30)
    stp     x0, x1, [sp, #-16]!
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp

    // 2. Pass ESR_EL1 and ELR_EL1 as arguments to our C handler
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

/* All other unhandled exceptions will just hang here */
unhandled_exception:
    b       . // Infinite loop
