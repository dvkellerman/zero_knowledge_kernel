/* 
 * Serial Port (COM1) Implementation
 * 
 * This file implements serial port output functions.
 * COM1 is typically at I/O port 0x3F8.
 */

#include "serial.h"

/* Check if serial port is ready to transmit */
static int serial_is_transmit_empty(void) {
    unsigned char status;
    __asm__ volatile ("inb %1, %0" : "=a"(status) : "Nd"((unsigned short)(SERIAL_COM1_BASE + 5)));
    return (status & SERIAL_LINE_STATUS_THRE) != 0;
}

/* Initialize serial port COM1 */
void serial_init(void) {
    /* Disable interrupts */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0x00), "Nd"((unsigned short)(SERIAL_COM1_BASE + 1)));
    
    /* Enable DLAB (Divisor Latch Access Bit) to set baud rate */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0x80), "Nd"((unsigned short)(SERIAL_COM1_BASE + 3)));
    
    /* Set divisor to 3 (38400 baud) - low byte */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0x03), "Nd"((unsigned short)SERIAL_COM1_BASE));
    
    /* Set divisor - high byte */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0x00), "Nd"((unsigned short)(SERIAL_COM1_BASE + 1)));
    
    /* 8 bits, no parity, one stop bit - disable DLAB */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0x03), "Nd"((unsigned short)(SERIAL_COM1_BASE + 3)));
    
    /* Enable FIFO, clear them, with 14-byte threshold */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0xC7), "Nd"((unsigned short)(SERIAL_COM1_BASE + 2)));
    
    /* Enable interrupts, RTS/DSR set */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0x0B), "Nd"((unsigned short)(SERIAL_COM1_BASE + 4)));
}

/* Write a character to serial port */
void serial_putchar(char c) {
    /* Wait until transmitter is ready */
    while (!serial_is_transmit_empty()) {
        /* Busy wait */
    }
    
    /* Send the character */
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)c), "Nd"((unsigned short)SERIAL_COM1_BASE));
}

/* Write a string to serial port */
void serial_puts(const char* str) {
    for (unsigned int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            serial_putchar('\r');  /* Carriage return before newline */
        }
        serial_putchar(str[i]);
    }
}

/* Print unsigned integer to serial port */
void serial_putuint(unsigned int num) {
    if (num == 0) {
        serial_putchar('0');
        return;
    }
    
    char buffer[12];
    int i = 0;
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    for (int j = i - 1; j >= 0; j--) {
        serial_putchar(buffer[j]);
    }
}

/* Print hexadecimal to serial port */
void serial_puthex(unsigned int num) {
    char hex_chars[] = "0123456789ABCDEF";
    serial_puts("0x");
    
    if (num == 0) {
        serial_putchar('0');
        return;
    }
    
    int started = 0;
    for (int i = 7; i >= 0; i--) {
        unsigned char nibble = (num >> (i * 4)) & 0xF;
        if (nibble != 0 || started || i == 0) {
            serial_putchar(hex_chars[nibble]);
            started = 1;
        }
    }
}

