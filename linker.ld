/* 
 * Linker Script for x86 Kernel
 * 
 * This script tells the linker how to arrange our kernel in memory.
 * The kernel will be loaded at physical address 1MB (0x100000) by GRUB.
 */

/* Entry point - where execution begins */
ENTRY(_start)

/* Define memory layout */
SECTIONS
{
    /* Kernel starts at 1MB (0x100000) - standard location for kernels */
    . = 0x100000;
    
    /* Multiboot header MUST be in the first 8KB */
    .multiboot : {
        *(.multiboot)
    }
    
    /* Code section - executable code */
    .text : ALIGN(4K) {
        *(.text)
    }
    
    /* Read-only data section */
    .rodata : ALIGN(4K) {
        *(.rodata)
    }
    
    /* Read-write data section (initialized) */
    .data : ALIGN(4K) {
        *(.data)
    }
    
    /* BSS section - uninitialized data (should be zeroed) */
    .bss : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }
    
    /* End of kernel - useful for calculating kernel size */
    kernel_end = .;
}

