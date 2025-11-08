/* 
 * VGA Text Mode Header
 * 
 * This header provides functions for writing to the VGA text buffer.
 * VGA text mode uses a memory-mapped buffer at address 0xB8000.
 * 
 * Each character on screen is represented by 2 bytes:
 * - Byte 0: ASCII character code
 * - Byte 1: Color attribute (foreground + background)
 */

#ifndef VGA_H
#define VGA_H

/* VGA Text Mode Constants */
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

/* Color attributes for VGA text mode */
#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_BROWN         6
#define COLOR_LIGHT_GREY    7
#define COLOR_DARK_GREY     8
#define COLOR_LIGHT_BLUE    9
#define COLOR_LIGHT_GREEN   10
#define COLOR_LIGHT_CYAN    11
#define COLOR_LIGHT_RED     12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

/* Helper macro to create a color attribute byte */
#define VGA_COLOR(fg, bg) ((fg) | ((bg) << 4))

/* VGA Functions */

/* Clear the VGA screen */
void vga_clear(void);

/* Set the VGA color scheme */
void vga_set_color(unsigned char color);

/* Get the current VGA color */
unsigned char vga_get_color(void);

/* Write a single character to the VGA buffer */
void vga_putchar(char c);

/* Write a null-terminated string to the VGA buffer */
void vga_puts(const char* str);

/* Print an unsigned integer as decimal */
void vga_putuint(unsigned int num);

/* Print an unsigned integer as hexadecimal */
void vga_puthex(unsigned int num);

#endif /* VGA_H */

