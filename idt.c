/* 
 * Interrupt Descriptor Table (IDT) Implementation
 * 
 * This file implements the IDT setup and interrupt handling.
 */

#include "idt.h"
#include "debug.h"
#include "pic.h"

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

/* IRQ handlers (32-47) */
extern void isr32(void);  /* IRQ 0 - Timer */
extern void isr33(void);  /* IRQ 1 - Keyboard */
extern void isr34(void);  /* IRQ 2 - Cascade */
extern void isr35(void);  /* IRQ 3 - COM2 */
extern void isr36(void);  /* IRQ 4 - COM1 */
extern void isr37(void);  /* IRQ 5 - LPT2 */
extern void isr38(void);  /* IRQ 6 - Floppy */
extern void isr39(void);  /* IRQ 7 - LPT1 */
extern void isr40(void);  /* IRQ 8 - RTC */
extern void isr41(void);  /* IRQ 9 - Free */
extern void isr42(void);  /* IRQ 10 - Free */
extern void isr43(void);  /* IRQ 11 - Free */
extern void isr44(void);  /* IRQ 12 - PS/2 Mouse */
extern void isr45(void);  /* IRQ 13 - FPU */
extern void isr46(void);  /* IRQ 14 - Primary ATA */
extern void isr47(void);  /* IRQ 15 - Secondary ATA */

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

/* IRQ handler (called from assembly stubs for IRQs 32-47) */
void irq_handler(unsigned int interrupt_num) {
    /* Convert interrupt vector to IRQ number */
    unsigned char irq = interrupt_num - PIC_IRQ_BASE;
    
    /* For now, just print the IRQ */
    debug_info("IRQ received: ");
    debug_putuint(irq);
    debug_puts("\n");
    
    /* Send End of Interrupt to PIC */
    pic_send_eoi(irq);
    
    /* Note: We return from interrupt here (handled by assembly stub) */
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
    
    /* Set up IRQ handlers (32-47) */
    idt_set_entry(32, (unsigned int)isr32, 0x08, 0x8E);  /* IRQ 0 - Timer */
    idt_set_entry(33, (unsigned int)isr33, 0x08, 0x8E);  /* IRQ 1 - Keyboard */
    idt_set_entry(34, (unsigned int)isr34, 0x08, 0x8E);  /* IRQ 2 - Cascade */
    idt_set_entry(35, (unsigned int)isr35, 0x08, 0x8E);  /* IRQ 3 - COM2 */
    idt_set_entry(36, (unsigned int)isr36, 0x08, 0x8E);  /* IRQ 4 - COM1 */
    idt_set_entry(37, (unsigned int)isr37, 0x08, 0x8E);  /* IRQ 5 - LPT2 */
    idt_set_entry(38, (unsigned int)isr38, 0x08, 0x8E);  /* IRQ 6 - Floppy */
    idt_set_entry(39, (unsigned int)isr39, 0x08, 0x8E);  /* IRQ 7 - LPT1 */
    idt_set_entry(40, (unsigned int)isr40, 0x08, 0x8E);  /* IRQ 8 - RTC */
    idt_set_entry(41, (unsigned int)isr41, 0x08, 0x8E);  /* IRQ 9 - Free */
    idt_set_entry(42, (unsigned int)isr42, 0x08, 0x8E);  /* IRQ 10 - Free */
    idt_set_entry(43, (unsigned int)isr43, 0x08, 0x8E);  /* IRQ 11 - Free */
    idt_set_entry(44, (unsigned int)isr44, 0x08, 0x8E);  /* IRQ 12 - PS/2 Mouse */
    idt_set_entry(45, (unsigned int)isr45, 0x08, 0x8E);  /* IRQ 13 - FPU */
    idt_set_entry(46, (unsigned int)isr46, 0x08, 0x8E);  /* IRQ 14 - Primary ATA */
    idt_set_entry(47, (unsigned int)isr47, 0x08, 0x8E);  /* IRQ 15 - Secondary ATA */
    
    /* Load IDT */
    __asm__ volatile ("lidt %0" : : "m"(idt_reg));
    
    debug_info("IDT initialized");
}

/* Register an interrupt handler (for future use) */
void idt_register_handler(unsigned char num, interrupt_handler_t handler) {
    idt_set_entry(num, (unsigned int)handler, 0x08, 0x8E);
}

