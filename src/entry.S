.section ".text.boot"

.global _start
.global irq_handler_stub

// Vector Table must be aligned to 2048 bytes
.align 11
.global vectors
vectors:
    // Current EL with SP0
    .align 7
    b .
    .align 7
    b .
    .align 7
    b .
    .align 7
    b .

    // Current EL with SPx (This is where our OS runs)
    .align 7
    b .             // Synchronous
    .align 7
    b irq_handler_stub // IRQ
    .align 7
    b .             // FIQ
    .align 7
    b .             // SError

    // Lower EL using AArch64
    .align 7
    b .
    .align 7
    b .
    .align 7
    b .
    .align 7
    b .

// The actual entry point
_start:
    ldr x0, =_stack_top
    mov sp, x0

    // Set VBAR_EL1 to point to our vector table
    ldr x0, =vectors
    msr vbar_el1, x0

    bl kernel_main
    b .

// Context switching wrapper for the IRQ
irq_handler_stub:
    // Save registers (Context Save)
    sub sp, sp, #256
    stp x0, x1, [sp, #16 * 0]
    stp x2, x3, [sp, #16 * 1]
    stp x4, x5, [sp, #16 * 2]
    stp x6, x7, [sp, #16 * 3]
    stp x8, x9, [sp, #16 * 4]
    stp x10, x11, [sp, #16 * 5]
    stp x12, x13, [sp, #16 * 6]
    stp x14, x15, [sp, #16 * 7]
    stp x16, x17, [sp, #16 * 8]
    stp x18, x30, [sp, #16 * 9] // LR is important!

    // Call C handler
    bl handle_irq

    // Restore registers (Context Restore)
    ldp x0, x1, [sp, #16 * 0]
    ldp x2, x3, [sp, #16 * 1]
    ldp x4, x5, [sp, #16 * 2]
    ldp x6, x7, [sp, #16 * 3]
    ldp x8, x9, [sp, #16 * 4]
    ldp x10, x11, [sp, #16 * 5]
    ldp x12, x13, [sp, #16 * 6]
    ldp x14, x15, [sp, #16 * 7]
    ldp x16, x17, [sp, #16 * 8]
    ldp x18, x30, [sp, #16 * 9]
    add sp, sp, #256
    eret
