.section ".text.boot"
.global _start

// ------------------------------------------------------------
// Constants
// ------------------------------------------------------------
.equ MAIR_VAL,      (0x00 | (0xFF << 8)) // Idx 0: Device, Idx 1: Normal

// TCR_EL1 configuration for 39-bit VA (512GB)
// T0SZ=25, T1SZ=25 (64-25 = 39 bits)
// TG0=4KB, TG1=4KB
.equ TCR_VAL,       (25 | (25 << 16) | (0 << 14) | (2 << 30) | (2 << 32))

// SCTLR_EL1: MMU + Caches
.equ SCTLR_VAL,     0x1005 // M=1, C=1, I=1

// Page Table Attributes (Level 1 Block Descriptors)
// 0x701 = Block | AF | InnerShare | Normal Memory
.equ PT_MEM,        0x705 
// 0x700 = Block | AF | InnerShare | Device Memory (Wait, device shouldn't be shareable usually, but simplified here)
// Better Device Attr: Block(1)|AttrIdx0(0)|AF(1<<10)|UXN|PXN -> 0x401
.equ PT_DEV,        0x401 

_start:
    // 1. Temporary Stack (Physical Address)
    // We use adr to get the physical address of the label, 
    // because sp must be valid before MMU is on.
    adr x0, _stack_top_phys
    mov sp, x0

    // 2. Clear Page Tables (8KB total)
    adr x0, idmap_dir  // Use ADR to get physical address of table
    mov x1, #0x2000
    mov x2, #0
clear_loop:
    str xzr, [x0], #8
    subs x1, x1, #8
    b.gt clear_loop

    // ------------------------------------------------------------
    // 3. Setup TTBR0 (Identity Map - Lower Half)
    // ------------------------------------------------------------
    // We map 0x40000000 (RAM) -> 0x40000000 (Physical)
    // This allows the code to keep running "in place" when MMU turns on.
    
    adr x0, idmap_dir
    mov x1, #0x40000000     // Phys Addr
    ldr x2, =PT_MEM
    orr x1, x1, x2          // Combine
    str x1, [x0, #8]        // Index 1 (covers 1GB-2GB)

    // Also Map UART (0x09000000) in Lower half for debugging
    mov x1, #0x00000000     // Phys Addr Base 0
    ldr x2, =PT_DEV
    orr x1, x1, x2
    str x1, [x0, #0]        // Index 0 (covers 0-1GB)

    // ------------------------------------------------------------
    // 4. Setup TTBR1 (Kernel Map - Upper Half)
    // ------------------------------------------------------------
    // Virtual Base: 0xFFFFFF80 00000000
    // We want to map:
    //   Virtual [Base + 0GB] -> Phys 0x00000000 (Device/UART)
    //   Virtual [Base + 1GB] -> Phys 0x40000000 (RAM/Kernel)

    adr x0, highmap_dir
    
    // Entry 0: Device (For UART access in high kernel)
    mov x1, #0
    ldr x2, =PT_DEV
    orr x1, x1, x2
    str x1, [x0, #0]

    // Entry 1: RAM (For Kernel Code)
    mov x1, #0x40000000
    ldr x2, =PT_MEM
    orr x1, x1, x2
    str x1, [x0, #8]

    // ------------------------------------------------------------
    // 5. Activate MMU
    // ------------------------------------------------------------
    ldr x0, =MAIR_VAL
    msr mair_el1, x0
    
    ldr x0, =TCR_VAL
    msr tcr_el1, x0

    // Load Physical Addresses of tables into registers
    adr x0, idmap_dir
    msr ttbr0_el1, x0
    
    adr x0, highmap_dir
    msr ttbr1_el1, x0

    // Enable
    ldr x0, =SCTLR_VAL
    mrs x1, sctlr_el1
    orr x1, x1, x0
    msr sctlr_el1, x1
    isb

    // ------------------------------------------------------------
    // 6. The Trampoline Jump
    // ------------------------------------------------------------
    // We are currently running in Identity Map (Low).
    // We need to jump to the High Map.
    
    // 'kernel_main' symbol is linked at 0xFFFFFF80....
    // ldr will load that huge number into x0.
    ldr x0, =kernel_main 
    
    // Update Stack Pointer to High Address
    ldr x1, =_stack_top
    mov sp, x1

    // Jump
    br x0

    // Catch if return
halt:
    wfe
    b halt

// Helper to calculate stack physical location for init
.align 3
_stack_top_phys:
    .quad _stack_top - 0xFFFFFF8000000000
