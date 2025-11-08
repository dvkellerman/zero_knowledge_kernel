/* 
 * Interrupt Descriptor Table (IDT) Implementation
 * 
 * This file implements the IDT setup and interrupt handling.
 */

#include "idt.h"
#include "debug.h"

/* Forward declaration for halt() */
extern void halt(void);

/* IDT with 256 entries */
#define IDT_ENTRIES 256
static struct idt_entry idt[IDT_ENTRIES];
static struct idt_register idt_reg;

/* Exception names for debugging */
const char* exception_names[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* Forward declarations for interrupt handler stubs */
/* These are defined in assembly (we'll create them) */
extern void isr0(void);   /* Division by zero */
extern void isr1(void);   /* Debug */
extern void isr2(void);   /* Non-maskable interrupt */
extern void isr3(void);   /* Breakpoint */
extern void isr4(void);   /* Overflow */
extern void isr5(void);   /* Bound range exceeded */
extern void isr6(void);   /* Invalid opcode */
extern void isr7(void);   /* Device not available */
extern void isr8(void);   /* Double fault */
extern void isr9(void);   /* Coprocessor segment overrun */
extern void isr10(void);  /* Invalid TSS */
extern void isr11(void);  /* Segment not present */
extern void isr12(void);  /* Stack fault */
extern void isr13(void);  /* General protection fault */
extern void isr14(void);  /* Page fault */
extern void isr15(void);  /* Reserved */
extern void isr16(void);  /* x87 FPU error */
extern void isr17(void);  /* Alignment check */
extern void isr18(void);  /* Machine check */
extern void isr19(void);  /* SIMD floating point exception */
/* We'll add more as needed */

/* Generic exception handler (called from assembly stubs) */
void exception_handler(unsigned int interrupt_num) {
    debug_error("Exception occurred!");
    
    if (interrupt_num < 32) {
        debug_puts("Exception: ");
        debug_puts(exception_names[interrupt_num]);
        debug_puts(" (");
        debug_putuint(interrupt_num);
        debug_puts(")\n");
    } else {
        debug_puts("Interrupt: ");
        debug_putuint(interrupt_num);
        debug_puts("\n");
    }
    
    /* For now, we'll halt on exceptions */
    /* Later we can add proper error recovery */
    halt();
}

/* Set an IDT entry */
void idt_set_entry(unsigned char num, unsigned int handler, unsigned short selector, unsigned char flags) {
    idt[num].offset_low = handler & 0xFFFF;
    idt[num].offset_high = (handler >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

/* Initialize and load the IDT */
void idt_init(void) {
    /* Set up IDT register */
    idt_reg.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idt_reg.base = (unsigned int)&idt;
    
    /* Clear all IDT entries first */
    unsigned int i;
    for (i = 0; i < IDT_ENTRIES; i++) {
        idt_set_entry(i, 0, 0, 0);
    }
    
    /* Set up exception handlers (0-31) */
    /* Flags: 0x8E = Present (bit 7), DPL 00 (bits 6-5), 32-bit interrupt gate (bits 4-0) */
    /* Selector: 0x08 = Kernel code segment (set up by GRUB) */
    idt_set_entry(0, (unsigned int)isr0, 0x08, 0x8E);
    idt_set_entry(1, (unsigned int)isr1, 0x08, 0x8E);
    idt_set_entry(2, (unsigned int)isr2, 0x08, 0x8E);
    idt_set_entry(3, (unsigned int)isr3, 0x08, 0x8E);
    idt_set_entry(4, (unsigned int)isr4, 0x08, 0x8E);
    idt_set_entry(5, (unsigned int)isr5, 0x08, 0x8E);
    idt_set_entry(6, (unsigned int)isr6, 0x08, 0x8E);
    idt_set_entry(7, (unsigned int)isr7, 0x08, 0x8E);
    idt_set_entry(8, (unsigned int)isr8, 0x08, 0x8E);
    idt_set_entry(9, (unsigned int)isr9, 0x08, 0x8E);
    idt_set_entry(10, (unsigned int)isr10, 0x08, 0x8E);
    idt_set_entry(11, (unsigned int)isr11, 0x08, 0x8E);
    idt_set_entry(12, (unsigned int)isr12, 0x08, 0x8E);
    idt_set_entry(13, (unsigned int)isr13, 0x08, 0x8E);
    idt_set_entry(14, (unsigned int)isr14, 0x08, 0x8E);
    idt_set_entry(15, (unsigned int)isr15, 0x08, 0x8E);
    idt_set_entry(16, (unsigned int)isr16, 0x08, 0x8E);
    idt_set_entry(17, (unsigned int)isr17, 0x08, 0x8E);
    idt_set_entry(18, (unsigned int)isr18, 0x08, 0x8E);
    idt_set_entry(19, (unsigned int)isr19, 0x08, 0x8E);
    
    /* Load IDT */
    __asm__ volatile ("lidt %0" : : "m"(idt_reg));
    
    debug_info("IDT initialized");
}

/* Register an interrupt handler (for future use) */
void idt_register_handler(unsigned char num, interrupt_handler_t handler) {
    idt_set_entry(num, (unsigned int)handler, 0x08, 0x8E);
}

