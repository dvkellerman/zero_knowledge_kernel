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
 * Interrupt Handler Stub Macro (for exceptions WITHOUT error code)
 * 
 * Creates an interrupt handler stub that:
 * 1. Saves all registers
 * 2. Pushes the interrupt number
 * 3. Calls the C exception handler
 * 4. Restores registers
 * 5. Returns from interrupt
 * 
 * Note: The CPU automatically pushes EFLAGS, CS, and EIP when an interrupt occurs.
 * Most exceptions do NOT push an error code.
 * 
 * Stack layout on entry (from CPU):
 * - [EIP] (4 bytes)
 * - [CS] (4 bytes)
 * - [EFLAGS] (4 bytes)
 */
#define ISR_STUB(num) \
void isr##num(void) { \
    __asm__ volatile ( \
        "pusha\n"              /* Save all general-purpose registers (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI) */ \
        "pushl $" #num "\n"    /* Push interrupt number as parameter */ \
        "call exception_handler\n" \
        "addl $4, %%esp\n"     /* Remove interrupt number from stack (clean up parameter) */ \
        "popa\n"               /* Restore all general-purpose registers */ \
        "iret\n"               /* Return from interrupt (pops EIP, CS, EFLAGS) */ \
        ::: "memory" \
    ); \
}

/* 
 * Interrupt Handler Stub Macro (for exceptions WITH error code)
 * 
 * Some exceptions (8, 10-14, 17) automatically push an error code.
 * We need to pop this error code before doing iret.
 * 
 * Stack layout on entry (from CPU):
 * - [Error Code] (4 bytes) - pushed by CPU automatically
 * - [EIP] (4 bytes)
 * - [CS] (4 bytes)
 * - [EFLAGS] (4 bytes)
 */
#define ISR_STUB_ERROR_CODE(num) \
void isr##num(void) { \
    __asm__ volatile ( \
        "pusha\n"              /* Save all general-purpose registers */ \
        "pushl $" #num "\n"    /* Push interrupt number as parameter */ \
        "call exception_handler\n" \
        "addl $4, %%esp\n"     /* Remove interrupt number from stack */ \
        "popa\n"               /* Restore all general-purpose registers */ \
        "addl $4, %%esp\n"     /* Remove error code from stack (exceptions with error code push it automatically) */ \
        "iret\n"               /* Return from interrupt (pops EIP, CS, EFLAGS) */ \
        ::: "memory" \
    ); \
}

/* Create interrupt stubs for exceptions 0-19 */
/* Exceptions 0-7: No error code */
ISR_STUB(0)   /* Division by zero */
ISR_STUB(1)   /* Debug */
ISR_STUB(2)   /* Non-maskable interrupt */
ISR_STUB(3)   /* Breakpoint */
ISR_STUB(4)   /* Overflow */
ISR_STUB(5)   /* Bound range exceeded */
ISR_STUB(6)   /* Invalid opcode */
ISR_STUB(7)   /* Device not available */
/* Exception 8: Double fault - HAS error code */
ISR_STUB_ERROR_CODE(8)
/* Exception 9: Coprocessor segment overrun - No error code */
ISR_STUB(9)
/* Exceptions 10-14: Invalid TSS, Segment Not Present, Stack Fault, GPF, Page Fault - HAVE error codes */
ISR_STUB_ERROR_CODE(10)  /* Invalid TSS */
ISR_STUB_ERROR_CODE(11)  /* Segment not present */
ISR_STUB_ERROR_CODE(12)  /* Stack fault */
ISR_STUB_ERROR_CODE(13)  /* General protection fault */
ISR_STUB_ERROR_CODE(14)  /* Page fault */
/* Exception 15: Reserved - No error code */
ISR_STUB(15)
/* Exception 16: x87 FPU error - No error code */
ISR_STUB(16)
/* Exception 17: Alignment check - HAS error code */
ISR_STUB_ERROR_CODE(17)
/* Exceptions 18-19: Machine check, SIMD FP exception - No error code */
ISR_STUB(18)  /* Machine check */
ISR_STUB(19)  /* SIMD floating point exception */

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

