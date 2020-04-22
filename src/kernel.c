#include <kernel.h>

void kernel_main(void)
{
    vga_init();
    vga_printf("------------------------------------------------------------\n");
    vga_printf("                  Welcome to tarediiran-os                  \n");
    vga_printf("------------------------------------------------------------\n");

    gdt_init();
    vga_printf("Global descriptor table initialized!\n");
    
    for (;;);
}
