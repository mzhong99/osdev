#include <kernel/vga.h>
#include <kernel/io.h>

#define VIDEO_MEMORY ((uint16_t *) 0x000B8000)

#define VGA_CMD_PORT    0x3D4
#define VGA_DATA_PORT   0x3D5

#define VGA_HIGH_BYTE_CMD   14
#define VGA_LOW_BYTE_CMD    15

static struct vga self;

static uint8_t vga_entry_color(enum vga_color fg_color, enum vga_color bg_color)
{
    return (bg_color << 4) | fg_color;
}

static uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t) c | (uint16_t) (color << 8);
}

static void vga_display_buffer()
{
    uint8_t entry_color = vga_entry_color(self.fg_color, self.bg_color);

    for (size_t row = 0; row < VGA_ROWS; row++) 
    {
        for (size_t col = 0; col < VGA_COLS; col++) 
        {
            size_t index = row * VGA_COLS + col;
            self.video_memory[index] = vga_entry(self.buffer[row][col], entry_color);
        }
    }
}

static void vga_advance_row()
{
    self.row++;
    self.col = 0;

    if (self.row >= VGA_ROWS)
    {
        self.row = VGA_ROWS - 1;
        for (size_t row = 0; row < VGA_ROWS - 1; row++) {
            for (size_t col = 0; col < VGA_COLS; col++) {
                self.buffer[row][col] = self.buffer[row + 1][col];
            }
        }
    }

    for (size_t col = 0; col < VGA_COLS; col++) {
        self.buffer[self.row][col] = ' ';
    }
}


void vga_init(void)
{
    self.row = 0;
    self.col = 0;
    self.video_memory = VIDEO_MEMORY;
    self.fg_color = VGA_COLOR_BLACK;
    self.bg_color = VGA_COLOR_LIGHT_GREY;

    for (size_t row = 0; row < VGA_ROWS; row++) {
        for (size_t col = 0; col < VGA_COLS; col++) {
            self.buffer[row][col] = ' ';
        }
    }

    vga_display_buffer();
}

static void vga_move_cursor(uint16_t row, uint16_t col)
{
    uint16_t position = row * VGA_COLS + col;

    io_sendbyte(VGA_CMD_PORT, VGA_HIGH_BYTE_CMD);
    io_sendbyte(VGA_DATA_PORT, (position >> 8) & 0xFF);
    io_sendbyte(VGA_CMD_PORT, VGA_LOW_BYTE_CMD);
    io_sendbyte(VGA_DATA_PORT, (position & 0xFF));
}

static void vga_putchar(char c)
{
    switch (c)
    {
        case '\n':
            vga_advance_row();
            break;

        default:
            self.buffer[self.row][self.col++] = c;
            if (self.col >= VGA_COLS) { vga_advance_row(); }
            break;
    }

    vga_move_cursor(self.row, self.col);
    vga_display_buffer();
}

static char * convert(uint32_t number, uint32_t base, int32_t reqlen)
{
    static char representation[] = "0123456789ABCDEF";
    static char buffer[50];
    
    char * ptr = &buffer[50];
    *ptr = '\0';

    while (number != 0 || reqlen > 0)
    {
        *--ptr = representation[number % base];
        number /= base;

        reqlen--;
    }

    return ptr;
}

void vga_printf(const char * restrict format, ...)
{
    va_list args;
    va_start(args, format);

    for (char * str = (char *) format; *str != '\0'; str++) 
    {
        if (*str != '%') 
        {
            vga_putchar(*str);
        }
        else
        {
            str++;
            switch (*str)
            {
                case 'd':
                {
                    int32_t number = va_arg(args, int32_t);
                    if (number < 0) 
                    {
                        vga_putchar('-');
                        number = -number;
                    }
                    vga_puts(convert(number, 10, 1));
                    break;
                }

                case 's':
                    vga_puts(va_arg(args, char *));
                    break;

                case 'x':
                {
                    vga_puts("0x");
                    uint32_t number = va_arg(args, uint32_t);
                    vga_puts(convert(number, 16, 1));
                    break;
                }

                case 'p':
                {
                    vga_puts("0x");
                    uint32_t number = (uint32_t) va_arg(args, void *);
                    vga_puts(convert(number, 16, 8));
                    break;
                }
            }
        }
    }

    va_end(args);
}

void vga_puts(char * str)
{
    while (*str != '\0') { vga_putchar(*str++); }
}

