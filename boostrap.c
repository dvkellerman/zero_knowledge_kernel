/* 
 * x86 Kernel Bootstrap Code
 * 
 * This file contains:
 * 1. Multiboot header (required for GRUB to load the kernel)
 * 2. Kernel entry point (first function called by bootloader)
 * 3. Kernel main function
 */

#include "debug.h"

/* Multiboot Specification Constants */
#define MULTIBOOT_HEADER_MAGIC      0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC  0x2BADB002
#define MULTIBOOT_HEADER_FLAGS      0x00000003  /* Align modules on page boundaries + provide memory map */

/* 
 * Multiboot Header Structure
 * 
 * The Multiboot header must be in the first 8KB of the kernel binary.
 * It tells GRUB how to load our kernel and what information to provide.
 * 
 * Structure (32 bytes total):
 * - magic: 0x1BADB002 (Multiboot magic number)
 * - flags: Bit flags indicating what features we want
 * - checksum: magic + flags + checksum must equal 0
 */
__attribute__((section(".multiboot")))
__attribute__((used))
__attribute__((aligned(4)))
static const unsigned int multiboot_header[] = {
    MULTIBOOT_HEADER_MAGIC,      /* Magic number */
    MULTIBOOT_HEADER_FLAGS,      /* Flags */
    -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)  /* Checksum */
};

/* Multiboot Information Structure
 * 
 * This structure is passed to our kernel by the bootloader.
 * It contains information about memory, boot device, command line, etc.
 */
struct multiboot_info {
    unsigned int flags;
    unsigned int mem_lower;      /* Lower memory (in KB) */
    unsigned int mem_upper;      /* Upper memory (in KB) */
    unsigned int boot_device;
    unsigned int cmdline;        /* Command line string */
    unsigned int mods_count;
    unsigned int mods_addr;
    unsigned int syms[4];        /* Symbol table info */
    unsigned int mmap_length;    /* Memory map length */
    unsigned int mmap_addr;      /* Memory map address */
    unsigned int drives_length;
    unsigned int drives_addr;
    unsigned int config_table;
    unsigned int boot_loader_name;
    unsigned int apm_table;
    unsigned int vbe_control_info;
    unsigned int vbe_mode_info;
    unsigned short vbe_mode;
    unsigned short vbe_interface_seg;
    unsigned short vbe_interface_off;
    unsigned short vbe_interface_len;
};

/* Forward declaration */
void kernel_main(unsigned int magic, struct multiboot_info* mbi);

/* ============================================================================
 * Panic and Halt Functions
 * ============================================================================
 * 
 * These are fundamental kernel functions for error handling.
 * They use the debug system for output.
 */

/* Halt the CPU indefinitely */
void halt(void) {
    /* Output using debug system */
    debug_set_color(VGA_COLOR(COLOR_WHITE, COLOR_RED));
    debug_puts("System halted!\n");
    
    /* Disable interrupts and halt CPU */
    while (1) {
        __asm__ volatile ("cli");      /* Disable interrupts */
        __asm__ volatile ("hlt");      /* Halt CPU */
    }
}

/* Panic - critical error, halt the system */
void panic(const char* message) {
    /* Output using debug system */
    debug_set_color(VGA_COLOR(COLOR_WHITE, COLOR_RED));
    debug_puts("[PANIC] ");
    debug_puts(message);
    debug_puts("\n");
    
    /* Halt the system */
    halt();
}

/* 
 * Entry point - called by the bootloader
 * 
 * The bootloader puts:
 * - Magic number in EAX register
 * - Multiboot info pointer in EBX register
 * 
 * We use inline assembly to get these values and call our C function.
 */
__attribute__((section(".text")))
void _start(void) {
    unsigned int magic;
    struct multiboot_info* mbi;
    
    /* Get magic number from EAX register */
    __asm__ volatile ("movl %%eax, %0" : "=r" (magic));
    
    /* Get multiboot info pointer from EBX register */
    __asm__ volatile ("movl %%ebx, %0" : "=r" (mbi));
    
    /* Call our main kernel function */
    kernel_main(magic, mbi);
}

/* Kernel entry point - called by the bootloader */
void kernel_main(unsigned int magic, struct multiboot_info* mbi) {
    /* Initialize debug system (initializes serial port) */
    debug_init();
    debug_info("Debug system initialized");
    
    /* Verify we were loaded by a Multiboot-compliant bootloader */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("Invalid bootloader magic number!");
    }
    
    debug_info("Multiboot magic verified");
    
    /* Clear the screen and set up colors */
    debug_clear();
    debug_set_color(VGA_COLOR(COLOR_LIGHT_GREEN, COLOR_BLACK));
    
    /* Print welcome message */
    debug_puts("Welcome to Zero Knowledge Kernel!\n");
    debug_puts("===================================\n\n");
    debug_info("Kernel starting...");
    
    /* Print bootloader information */
    debug_set_color(VGA_COLOR(COLOR_LIGHT_CYAN, COLOR_BLACK));
    debug_puts("Bootloader Magic: ");
    debug_puthex(magic);
    debug_puts("\n");
    
    /* Print memory information if available */
    if (mbi->flags & 0x01) {  /* Check if memory info is available */
        debug_set_color(VGA_COLOR(COLOR_YELLOW, COLOR_BLACK));
        debug_puts("Lower memory: ");
        debug_putuint(mbi->mem_lower);
        debug_puts(" KB\n");
        debug_puts("Upper memory: ");
        debug_putuint(mbi->mem_upper);
        debug_puts(" KB\n");
        
        debug_info("Memory information retrieved");
    } else {
        debug_warn("Memory information not available");
    }
    
    debug_set_color(VGA_COLOR(COLOR_WHITE, COLOR_BLACK));
    debug_puts("\nKernel initialized successfully!\n");
    debug_puts("System ready.\n");
    
    debug_info("Kernel initialized successfully");
    debug_info("System ready");
    
    /* Test debug logging */
    debug_debug("This is a debug message");
    debug_info("This is an info message");
    debug_warn("This is a warning message");
    debug_error("This is an error message (test)");
    
    /* Infinite loop - kernel is running */
    while (1) {
        /* For now, we just halt the CPU */
        __asm__ volatile ("hlt");
    }
}

