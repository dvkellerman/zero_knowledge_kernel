/* 
 * Timer (PIT - Programmable Interval Timer) Header
 * 
 * The PIT generates periodic interrupts that can be used for:
 * - Time tracking
 * - Task scheduling
 * - Timeouts
 * 
 * The PIT is at I/O ports 0x40-0x43.
 * Channel 0 is used for timer interrupts (IRQ 0).
 */

#ifndef TIMER_H
#define TIMER_H

/* PIT I/O Ports */
#define PIT_CHANNEL0_DATA    0x40  /* Channel 0 data port */
#define PIT_CHANNEL1_DATA    0x41  /* Channel 1 data port */
#define PIT_CHANNEL2_DATA    0x42  /* Channel 2 data port */
#define PIT_COMMAND          0x43  /* Command/control port */

/* PIT Command Bits */
#define PIT_CHANNEL0         0x00  /* Select channel 0 */
#define PIT_ACCESS_MODE_LOW  0x10  /* Access mode: low byte only */
#define PIT_ACCESS_MODE_HIGH 0x20  /* Access mode: high byte only */
#define PIT_ACCESS_MODE_BOTH 0x30  /* Access mode: both bytes */
#define PIT_MODE_RATE_GEN    0x04  /* Mode: rate generator */
#define PIT_MODE_SQUARE_WAVE 0x06  /* Mode: square wave generator */
#define PIT_BINARY_MODE      0x00  /* Binary mode (0-65535) */
#define PIT_BCD_MODE         0x01  /* BCD mode (0-9999) */

/* PIT Frequency */
#define PIT_BASE_FREQUENCY   1193182  /* PIT base frequency in Hz */
#define TIMER_FREQUENCY      100      /* Desired timer frequency in Hz (100 Hz = 10ms intervals) */
#define PIT_DIVISOR          (PIT_BASE_FREQUENCY / TIMER_FREQUENCY)  /* Divisor for desired frequency */

/* Timer Functions */

/* Initialize timer (PIT) */
void timer_init(void);

/* Get current tick count */
unsigned int timer_get_ticks(void);

/* Get time in milliseconds (approximate) */
unsigned int timer_get_ms(void);

/* Timer interrupt handler (called from IRQ handler) */
void timer_interrupt_handler(void);

#endif /* TIMER_H */

