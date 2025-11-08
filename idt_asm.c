/* 
 * Interrupt Handler Stubs (Assembly)
 * 
 * These are assembly stubs that handle interrupts and call our C handlers.
 * We use inline assembly to create these stubs.
 */

#include "idt.h"

/* Forward declarations */
void exception_handler(unsigned int interrupt_num);
void irq_handler(unsigned int interrupt_num);

/* 
 * Interrupt Handler Stub Macro
 * 
 * Creates an interrupt handler stub that:
 * 1. Saves all registers
 * 2. Pushes the interrupt number
 * 3. Calls the C exception handler
 * 4. Restores registers
 * 5. Returns from interrupt
 * 
 * Note: The CPU automatically pushes EFLAGS, CS, and EIP when an interrupt occurs.
 * Some exceptions also push an error code automatically.
 */
#define ISR_STUB(num) \
void isr##num(void) { \
    __asm__ volatile ( \
        "pusha\n"              /* Save all general-purpose registers */ \
        "pushl $" #num "\n"    /* Push interrupt number */ \
        "call exception_handler\n" \
        "addl $4, %%esp\n"     /* Remove interrupt number from stack */ \
        "popa\n"               /* Restore all general-purpose registers */ \
        "iret\n"               /* Return from interrupt (restores EFLAGS, CS, EIP) */ \
        ::: "memory" \
    ); \
}

/* Create interrupt stubs for exceptions 0-19 */
ISR_STUB(0)
ISR_STUB(1)
ISR_STUB(2)
ISR_STUB(3)
ISR_STUB(4)
ISR_STUB(5)
ISR_STUB(6)
ISR_STUB(7)
ISR_STUB(8)
ISR_STUB(9)
ISR_STUB(10)
ISR_STUB(11)
ISR_STUB(12)
ISR_STUB(13)
ISR_STUB(14)
ISR_STUB(15)
ISR_STUB(16)
ISR_STUB(17)
ISR_STUB(18)
ISR_STUB(19)

/* IRQ Handler Stub Macro - calls irq_handler instead of exception_handler */
#define IRQ_STUB(num) \
void isr##num(void) { \
    __asm__ volatile ( \
        "pusha\n"              /* Save all general-purpose registers */ \
        "pushl $" #num "\n"    /* Push interrupt number */ \
        "call irq_handler\n"   /* Call IRQ handler (sends EOI) */ \
        "addl $4, %%esp\n"     /* Remove interrupt number from stack */ \
        "popa\n"               /* Restore all general-purpose registers */ \
        "iret\n"               /* Return from interrupt (restores EFLAGS, CS, EIP) */ \
        ::: "memory" \
    ); \
}

/* Create interrupt stubs for IRQs 32-47 */
IRQ_STUB(32)
IRQ_STUB(33)
IRQ_STUB(34)
IRQ_STUB(35)
IRQ_STUB(36)
IRQ_STUB(37)
IRQ_STUB(38)
IRQ_STUB(39)
IRQ_STUB(40)
IRQ_STUB(41)
IRQ_STUB(42)
IRQ_STUB(43)
IRQ_STUB(44)
IRQ_STUB(45)
IRQ_STUB(46)
IRQ_STUB(47)

