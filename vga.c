/* 
 * VGA Text Mode Implementation
 * 
 * This file implements VGA text mode output functions.
 * VGA text mode uses a memory-mapped buffer at address 0xB8000.
 */

#include "vga.h"

/* VGA text buffer - each entry is 2 bytes: [character][color] */
static volatile unsigned short* vga_buffer = (volatile unsigned short*)VGA_MEMORY;
static unsigned int vga_row = 0;
static unsigned int vga_col = 0;
static unsigned char vga_color = VGA_COLOR(COLOR_LIGHT_GREY, COLOR_BLACK);

/* Clear the VGA screen */
void vga_clear(void) {
    unsigned short blank = (unsigned short)' ' | (vga_color << 8);
    for (unsigned int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    vga_row = 0;
    vga_col = 0;
}

/* Set the VGA color scheme */
void vga_set_color(unsigned char color) {
    vga_color = color;
}

/* Get the current VGA color */
unsigned char vga_get_color(void) {
    return vga_color;
}

/* Write a single character to the VGA buffer */
void vga_putchar(char c) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
        if (vga_row >= VGA_HEIGHT) {
            vga_row = 0;  /* Simple wrap-around */
        }
        return;
    }
    
    if (c == '\r') {
        vga_col = 0;
        return;
    }
    
    if (vga_col >= VGA_WIDTH) {
        vga_col = 0;
        vga_row++;
        if (vga_row >= VGA_HEIGHT) {
            vga_row = 0;
        }
    }
    
    unsigned int index = vga_row * VGA_WIDTH + vga_col;
    vga_buffer[index] = (unsigned short)c | (vga_color << 8);
    vga_col++;
}

/* Write a null-terminated string to the VGA buffer */
void vga_puts(const char* str) {
    for (unsigned int i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}

/* Print an unsigned integer as decimal */
void vga_putuint(unsigned int num) {
    if (num == 0) {
        vga_putchar('0');
        return;
    }
    
    /* Convert number to string (reverse order) */
    char buffer[12];  /* Enough for 32-bit unsigned int */
    int i = 0;
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    /* Print in reverse order */
    for (int j = i - 1; j >= 0; j--) {
        vga_putchar(buffer[j]);
    }
}

/* Print an unsigned integer as hexadecimal */
void vga_puthex(unsigned int num) {
    char hex_chars[] = "0123456789ABCDEF";
    vga_puts("0x");
    
    if (num == 0) {
        vga_putchar('0');
        return;
    }
    
    /* Skip leading zeros */
    int started = 0;
    for (int i = 7; i >= 0; i--) {
        unsigned char nibble = (num >> (i * 4)) & 0xF;
        if (nibble != 0 || started || i == 0) {
            vga_putchar(hex_chars[nibble]);
            started = 1;
        }
    }
}

