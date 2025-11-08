/* 
 * Serial Port (COM1) Header
 * 
 * This header provides functions for writing to the serial port.
 * COM1 is typically at I/O port 0x3F8.
 * 
 * QEMU can redirect serial port output to:
 * - stdout: -serial stdio
 * - A file: -serial file:serial.log
 * - A socket: -serial tcp:localhost:1234
 * 
 * This is extremely useful for kernel debugging because:
 * 1. It works even when VGA isn't available
 * 2. Output can be logged to files
 * 3. It doesn't interfere with the display
 */

#ifndef SERIAL_H
#define SERIAL_H

/* Serial port I/O addresses for COM1 */
#define SERIAL_COM1_BASE  0x3F8

/* Serial port registers (offset from base) */
#define SERIAL_DATA_PORT(base)      (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* Line status register bits */
#define SERIAL_LINE_STATUS_THRE  0x20  /* Transmitter Holding Register Empty */

/* Serial Port Functions */

/* Initialize serial port COM1 */
void serial_init(void);

/* Write a character to serial port */
void serial_putchar(char c);

/* Write a string to serial port */
void serial_puts(const char* str);

/* Print unsigned integer to serial port */
void serial_putuint(unsigned int num);

/* Print hexadecimal to serial port */
void serial_puthex(unsigned int num);

#endif /* SERIAL_H */

