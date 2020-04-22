#ifndef __VGA_H__
#define __VGA_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#define VGA_ROWS 25
#define VGA_COLS 80

// Hardware text mode color constants
enum vga_color
{
    VGA_COLOR_BLACK         = 0,
    VGA_COLOR_BLUE          = 1,
    VGA_COLOR_GREEN         = 2,
    VGA_COLOR_CYAN          = 3,
    VGA_COLOR_RED           = 4,
    VGA_COLOR_MAGENTA       = 5,
    VGA_COLOR_BROWN         = 6,
    VGA_COLOR_LIGHT_GREY    = 7,
    VGA_COLOR_DARK_GREY     = 8,
    VGA_COLOR_LIGHT_BLUE    = 9,
    VGA_COLOR_LIGHT_GREEN   = 10,
    VGA_COLOR_LIGHT_CYAN    = 11,
    VGA_COLOR_LIGHT_RED     = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN   = 14,
    VGA_COLOR_WHITE         = 15
};

struct vga
{
    char buffer[VGA_ROWS][VGA_COLS];
    uint16_t * video_memory;
    size_t row;
    size_t col;
    enum vga_color fg_color;
    enum vga_color bg_color;
};

void vga_init(void);
void vga_printf(const char * restrict format, ...);
void vga_puts(char * str);

#endif
