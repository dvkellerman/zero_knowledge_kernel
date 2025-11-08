/* 
 * Debug Logging System Implementation
 * 
 * This file implements unified output that goes to both VGA and serial port.
 * This is the main interface for all printing in the kernel.
 * Supports different log levels for filtering messages.
 */

#include "debug.h"
#include "vga.h"
#include "serial.h"

/* Current log level - only messages at or above this level will be shown */
static unsigned int current_log_level = LOG_DEBUG;

/* ============================================================================
 * Initialization
 * ============================================================================
 */

/* Initialize debug system (initializes serial port) */
void debug_init(void) {
    serial_init();
}

/* ============================================================================
 * General Printing Functions
 * ============================================================================
 * 
 * These functions output to both VGA and serial port.
 */

/* Clear the screen */
void debug_clear(void) {
    vga_clear();
}

/* Set the VGA color (only affects VGA output) */
void debug_set_color(unsigned char color) {
    vga_set_color(color);
}

/* Print a string */
void debug_puts(const char* str) {
    vga_puts(str);
    serial_puts(str);
}

/* Print an unsigned integer as decimal */
void debug_putuint(unsigned int num) {
    vga_putuint(num);
    serial_putuint(num);
}

/* Print an unsigned integer as hexadecimal */
void debug_puthex(unsigned int num) {
    vga_puthex(num);
    serial_puthex(num);
}

/* Set the minimum log level */
void debug_set_level(unsigned int level) {
    current_log_level = level;
}

/* Internal logging function */
static void debug_log_internal(unsigned int level, const char* prefix, const char* message) {
    if (level < current_log_level) {
        return;
    }
    
    /* Output to serial port (always) */
    serial_puts("[");
    serial_puts(prefix);
    serial_puts("] ");
    serial_puts(message);
    serial_puts("\n");
    
    /* Output to VGA (if level is INFO or higher) */
    if (level >= LOG_INFO) {
        unsigned char color;
        if (level == LOG_INFO) {
            color = VGA_COLOR(COLOR_LIGHT_GREY, COLOR_BLACK);
        } else if (level == LOG_WARN) {
            color = VGA_COLOR(COLOR_YELLOW, COLOR_BLACK);
        } else if (level == LOG_ERROR) {
            color = VGA_COLOR(COLOR_LIGHT_RED, COLOR_BLACK);
        } else {  /* LOG_PANIC */
            color = VGA_COLOR(COLOR_WHITE, COLOR_RED);
        }
        
        unsigned char old_color = vga_get_color();
        vga_set_color(color);
        vga_puts("[");
        vga_puts(prefix);
        vga_puts("] ");
        vga_puts(message);
        vga_puts("\n");
        vga_set_color(old_color);
    }
}

/* Debug logging macros */
#define debug_log(level, prefix, fmt, ...) do { \
    char buffer[256]; \
    debug_sprintf(buffer, fmt, ##__VA_ARGS__); \
    debug_log_internal(level, prefix, buffer); \
} while(0)

/* Simple sprintf implementation for debug messages */
void debug_sprintf(char* buffer, const char* format, ...) {
    /* For now, we'll implement a simple version */
    /* This is a placeholder - we'll expand it as needed */
    unsigned int i = 0;
    unsigned int j = 0;
    
    while (format[i] != '\0' && j < 255) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            /* Handle format specifiers - simplified version */
            if (format[i] == 's') {
                /* String - we'll need to pass this differently */
                buffer[j++] = '?';
            } else if (format[i] == 'd' || format[i] == 'u') {
                /* Integer - we'll need to pass this differently */
                buffer[j++] = '?';
            } else if (format[i] == 'x') {
                /* Hex - we'll need to pass this differently */
                buffer[j++] = '?';
            } else {
                buffer[j++] = format[i];
            }
        } else {
            buffer[j++] = format[i];
        }
        i++;
    }
    buffer[j] = '\0';
}

/* Simplified logging functions (without printf-style formatting for now) */
void debug_debug(const char* message) {
    debug_log_internal(LOG_DEBUG, "DEBUG", message);
}

void debug_info(const char* message) {
    debug_log_internal(LOG_INFO, "INFO", message);
}

void debug_warn(const char* message) {
    debug_log_internal(LOG_WARN, "WARN", message);
}

void debug_error(const char* message) {
    debug_log_internal(LOG_ERROR, "ERROR", message);
}


