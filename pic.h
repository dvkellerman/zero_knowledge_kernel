/* 
 * Programmable Interrupt Controller (PIC) Header
 * 
 * The PIC routes hardware interrupts (keyboard, timer, etc.) to the CPU.
 * There are two PICs: master (IRQ 0-7) and slave (IRQ 8-15).
 * 
 * We remap IRQs to interrupt vectors 32-47 to avoid conflicts with CPU exceptions (0-31).
 */

#ifndef PIC_H
#define PIC_H

/* PIC I/O Ports */
#define PIC1_COMMAND    0x20  /* Master PIC command port */
#define PIC1_DATA       0x21  /* Master PIC data port */
#define PIC2_COMMAND    0xA0  /* Slave PIC command port */
#define PIC2_DATA       0xA1  /* Slave PIC data port */

/* PIC Initialization Control Words (ICW) */
#define PIC_ICW1_INIT           0x10  /* Initialization command */
#define PIC_ICW1_ICW4           0x01  /* ICW4 needed */
#define PIC_ICW4_8086           0x01  /* 8086 mode */

/* PIC Commands */
#define PIC_EOI                 0x20  /* End of Interrupt */

/* IRQ to Interrupt Vector Mapping */
#define PIC_IRQ_BASE            32     /* Base interrupt vector for IRQs */
#define PIC1_OFFSET             32     /* Master PIC offset (IRQ 0-7 → 32-39) */
#define PIC2_OFFSET             40     /* Slave PIC offset (IRQ 8-15 → 40-47) */

/* PIC Functions */

/* Initialize and remap PIC */
void pic_init(void);

/* Enable a specific IRQ */
void pic_enable_irq(unsigned char irq);

/* Disable a specific IRQ */
void pic_disable_irq(unsigned char irq);

/* Send End of Interrupt to PIC */
void pic_send_eoi(unsigned char irq);

#endif /* PIC_H */

