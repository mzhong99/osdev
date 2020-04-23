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

    timers_init();
    vga_printf("Timers initialized!\n");

    struct software_timer timer = software_timer_construct_ms(1000);
    software_timer_start(&timer);

    for (;;)
    {
        if (software_timer_expired(&timer))
        {
            vga_printf("Software timer: %d -> %d\n", timer.start_ticks, timer.wait_ticks);
            software_timer_start(&timer);
        }
    }
}
