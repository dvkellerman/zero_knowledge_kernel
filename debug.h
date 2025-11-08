/* 
 * Debug Logging System Header
 * 
 * This header provides functions for unified output that goes to both
 * VGA and serial port. This is the main interface for all printing in the kernel.
 * Supports different log levels for filtering messages.
 */

#ifndef DEBUG_H
#define DEBUG_H

/* Log levels */
#define LOG_DEBUG   0
#define LOG_INFO    1
#define LOG_WARN    2
#define LOG_ERROR   3
#define LOG_PANIC   4

/* VGA Color Constants (exposed for convenience) */
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

/* ============================================================================
 * Initialization
 * ============================================================================
 */

/* Initialize debug system (initializes serial port) */
void debug_init(void);

/* ============================================================================
 * General Printing Functions
 * ============================================================================
 * 
 * These functions output to both VGA and serial port.
 */

/* Clear the screen */
void debug_clear(void);

/* Set the VGA color (only affects VGA output) */
void debug_set_color(unsigned char color);

/* Print a string */
void debug_puts(const char* str);

/* Print an unsigned integer as decimal */
void debug_putuint(unsigned int num);

/* Print an unsigned integer as hexadecimal */
void debug_puthex(unsigned int num);

/* ============================================================================
 * Debug Logging Functions
 * ============================================================================
 */

/* Set the minimum log level */
void debug_set_level(unsigned int level);

/* Simplified logging functions */
void debug_debug(const char* message);
void debug_info(const char* message);
void debug_warn(const char* message);
void debug_error(const char* message);

/* Simple sprintf implementation for debug messages */
void debug_sprintf(char* buffer, const char* format, ...);

#endif /* DEBUG_H */

