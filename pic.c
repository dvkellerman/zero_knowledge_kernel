/* 
 * Programmable Interrupt Controller (PIC) Implementation
 * 
 * This file implements PIC initialization and control.
 * The PIC routes hardware interrupts to the CPU.
 */

#include "pic.h"

/* Initialize and remap PIC */
void pic_init(void) {
    /* Save current interrupt masks */
    unsigned char a1, a2;
    __asm__ volatile ("inb %1, %0" : "=a"(a1) : "Nd"((unsigned short)PIC1_DATA));
    __asm__ volatile ("inb %1, %0" : "=a"(a2) : "Nd"((unsigned short)PIC2_DATA));
    
    /* Start initialization sequence (ICW1) */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)(PIC_ICW1_INIT | PIC_ICW1_ICW4)), "Nd"((unsigned short)PIC1_COMMAND));
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)(PIC_ICW1_INIT | PIC_ICW1_ICW4)), "Nd"((unsigned short)PIC2_COMMAND));
    
    /* ICW2: Set interrupt vector offsets */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)PIC1_OFFSET), "Nd"((unsigned short)PIC1_DATA));
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)PIC2_OFFSET), "Nd"((unsigned short)PIC2_DATA));
    
    /* ICW3: Configure master/slave connection */
    /* Master PIC: slave PIC is connected to IRQ 2 (bit 2 = 0x04) */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0x04), "Nd"((unsigned short)PIC1_DATA));
    /* Slave PIC: connected to master's IRQ 2 (value = 2) */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0x02), "Nd"((unsigned short)PIC2_DATA));
    
    /* ICW4: Set 8086 mode */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)PIC_ICW4_8086), "Nd"((unsigned short)PIC1_DATA));
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)PIC_ICW4_8086), "Nd"((unsigned short)PIC2_DATA));
    
    /* Restore interrupt masks (disable all interrupts initially) */
    __asm__ volatile ("outb %0, %1" : : "a"(a1), "Nd"((unsigned short)PIC1_DATA));
    __asm__ volatile ("outb %0, %1" : : "a"(a2), "Nd"((unsigned short)PIC2_DATA));
}

/* Enable a specific IRQ */
void pic_enable_irq(unsigned char irq) {
    unsigned short port;
    unsigned char value;
    
    if (irq < 8) {
        /* Master PIC */
        port = PIC1_DATA;
    } else {
        /* Slave PIC */
        port = PIC2_DATA;
        irq -= 8;
    }
    
    /* Read current mask */
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    
    /* Clear the bit for this IRQ (0 = enabled, 1 = disabled) */
    value &= ~(1 << irq);
    
    /* Write back */
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* Disable a specific IRQ */
void pic_disable_irq(unsigned char irq) {
    unsigned short port;
    unsigned char value;
    
    if (irq < 8) {
        /* Master PIC */
        port = PIC1_DATA;
    } else {
        /* Slave PIC */
        port = PIC2_DATA;
        irq -= 8;
    }
    
    /* Read current mask */
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    
    /* Set the bit for this IRQ (0 = enabled, 1 = disabled) */
    value |= (1 << irq);
    
    /* Write back */
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* Send End of Interrupt to PIC */
void pic_send_eoi(unsigned char irq) {
    if (irq >= 8) {
        /* If this is a slave PIC interrupt, send EOI to slave first */
        __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)PIC_EOI), "Nd"((unsigned short)PIC2_COMMAND));
    }
    
    /* Always send EOI to master PIC */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)PIC_EOI), "Nd"((unsigned short)PIC1_COMMAND));
}

