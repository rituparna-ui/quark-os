ENTRY(_start)

SECTIONS
{
    /* * The kernel is loaded at physical address 0x40000000 by QEMU.
     * However, we want the symbols to resolve to High Virtual Addresses.
     * Base of 39-bit upper region + 1GB (RAM offset) 
     */
    . = 0xFFFFFF8040000000;

    /* * AT(...) specifies the Load Memory Address (LMA).
     * This ensures the binary file is generated with physical offsets,
     * but code assumes it is running at the virtual address.
     */
    .text : AT(0x40000000) {
        KEEP(*(.text.boot))
        *(.text)
    }

    .rodata : { *(.rodata) }
    .data : { *(.data) }
    
    . = ALIGN(0x1000);
    .bss : {
        bss_begin = .;
        *(.bss)
        *(COMMON)
        bss_end = .;
    }

    /* * Reserve space for Page Tables.
     * We need 4KB alignment.
     */
    . = ALIGN(0x1000);
    idmap_dir = .;
    . += 0x1000;
    highmap_dir = .;
    . += 0x1000;

    /* Stack */
    . += 0x1000;
    _stack_top = .;
}
