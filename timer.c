/* 
 * Timer (PIT - Programmable Interval Timer) Implementation
 * 
 * This file implements timer functionality using the PIT.
 * The PIT generates periodic interrupts that we use for time tracking.
 */

#include "timer.h"
#include "pic.h"

/* Tick counter - incremented on each timer interrupt */
static volatile unsigned int timer_ticks = 0;

/* Initialize timer (PIT) */
void timer_init(void) {
    /* Configure PIT channel 0 for periodic interrupts */
    /* Command: Channel 0, Access mode both bytes, Mode 3 (square wave), Binary mode */
    unsigned char command = PIT_CHANNEL0 | PIT_ACCESS_MODE_BOTH | PIT_MODE_SQUARE_WAVE | PIT_BINARY_MODE;
    
    __asm__ volatile ("outb %0, %1" : : "a"(command), "Nd"((unsigned short)PIT_COMMAND));
    
    /* Set divisor (low byte then high byte) */
    unsigned short divisor = PIT_DIVISOR;
    unsigned char divisor_low = divisor & 0xFF;
    unsigned char divisor_high = (divisor >> 8) & 0xFF;
    
    __asm__ volatile ("outb %0, %1" : : "a"(divisor_low), "Nd"((unsigned short)PIT_CHANNEL0_DATA));
    __asm__ volatile ("outb %0, %1" : : "a"(divisor_high), "Nd"((unsigned short)PIT_CHANNEL0_DATA));
    
    /* Enable timer interrupt (IRQ 0) in PIC */
    pic_enable_irq(0);
}

/* Get current tick count */
unsigned int timer_get_ticks(void) {
    return timer_ticks;
}

/* Get time in milliseconds (approximate) */
unsigned int timer_get_ms(void) {
    /* Each tick is approximately 10ms (100 Hz = 10ms intervals) */
    return timer_ticks * (1000 / TIMER_FREQUENCY);
}

/* Timer interrupt handler (called from IRQ handler) */
void timer_interrupt_handler(void) {
    /* Increment tick counter */
    timer_ticks++;
    
    /* Note: We don't print here to avoid flooding the output */
    /* The IRQ handler will send EOI to PIC */
}

