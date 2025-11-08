/* 
 * Interrupt Descriptor Table (IDT) Header
 * 
 * The IDT is a table that tells the CPU where to jump when an interrupt
 * or exception occurs. It has 256 entries (0-255).
 * 
 * Entries 0-31: CPU exceptions (divide by zero, page fault, etc.)
 * Entries 32-255: Available for hardware interrupts and software interrupts
 */

#ifndef IDT_H
#define IDT_H

/* IDT Entry Structure (8 bytes)
 * 
 * Each entry describes an interrupt handler:
 * - Offset: Address of the interrupt handler (split into low 16 bits and high 16 bits)
 * - Selector: Code segment selector (we'll use kernel code segment)
 * - Flags: Type, privilege level, present bit
 */
struct idt_entry {
    unsigned short offset_low;   /* Lower 16 bits of handler address */
    unsigned short selector;     /* Code segment selector */
    unsigned char zero;           /* Must be zero */
    unsigned char flags;          /* Type, privilege, present */
    unsigned short offset_high;  /* Upper 16 bits of handler address */
} __attribute__((packed));

/* IDT Register Structure
 * 
 * This structure is loaded into the IDTR register with the `lidt` instruction.
 */
struct idt_register {
    unsigned short limit;         /* Size of IDT - 1 */
    unsigned int base;           /* Base address of IDT */
} __attribute__((packed));

/* IDT Functions */

/* Initialize and load the IDT */
void idt_init(void);

/* Set an IDT entry */
void idt_set_entry(unsigned char num, unsigned int handler, unsigned short selector, unsigned char flags);

/* Exception handler function type */
typedef void (*interrupt_handler_t)(void);

/* Register an interrupt handler */
void idt_register_handler(unsigned char num, interrupt_handler_t handler);

/* Exception names for debugging */
extern const char* exception_names[];

#endif /* IDT_H */

