#include <kernel.h>

void kernel_main(void)
{
    vga_init();
    vga_printf("--------------------------------------------------------------------------------");
    vga_printf("                            Welcome to tarediiran-os                            ");
    vga_printf("--------------------------------------------------------------------------------");

    gdt_init();
    vga_printf("Global descriptor table initialized!\n");

    pic_remap();
    vga_printf("PIC remapped!\n");

    idt_init();
    vga_printf("Interrupts enabled!\n");

    for (;;);
}
