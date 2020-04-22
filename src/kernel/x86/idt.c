#include <stddef.h>
#include <kernel/x86/idt.h>
#include <kernel/x86/pic.h>
#include <kernel/vga.h>

#define INTGATE  0x8E
#define TRAPGATE 0xEF

#define NUM_IDT_ENTRIES 256
#define NUM_IRQ_ENTRIES 16

void (* irq_routines[NUM_IRQ_ENTRIES]) (struct int_stackframe *);
static struct idt_entry idt[NUM_IDT_ENTRIES];
extern uintptr_t __interrupt_vector[];

static const char * exception_messages[32] = 
{
    "division by zero",
    "debug",
    "non-maskable interrupt",
    "breakpoint",
    "detected overflow",
    "out-of-bounds",
    "invalid opcode",
    "no coprocessor",
    "double fault",
    "coprocessor segment overrun",
    "bad TSS",
    "segment not present",
    "stack fault",
    "general protection fault",
    "page fault",
    "unknown interrupt",
    "coprocessor fault",
    "alignment check",
    "machine check",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved"
};

static void idt_entry_init(struct idt_entry * entry,
        uint32_t offset, uint16_t selector, uint8_t type)
{
    entry->offset_0to15 = (uint16_t) (offset & 0xFFFF);
    entry->offset_16to31 = (uint16_t) ((offset >> 16) & 0xFFFF);
    entry->zero = 0;
    entry->type_attr = type;
    entry->selector = selector;
}

void idt_init()
{
    for (size_t i = 0; i < NUM_IRQ_ENTRIES; i++) {
        irq_routines[i] = NULL;
    }

    for (size_t i = 0; i < 48; i++) {
        idt_entry_init(&idt[i], __interrupt_vector[i], 0x08, INTGATE);
    }

    idt_entry_init(&idt[128], __interrupt_vector[48], 0x08, TRAPGATE);

    struct idt_ptr ptr;
    ptr.length = sizeof(struct idt_entry) * NUM_IDT_ENTRIES;
    ptr.startaddr = (uint32_t) &idt[0];

    extern void idt_flush(uint32_t);
    idt_flush((uint32_t) &ptr);

    asm volatile ("sti");
}

void print_stackframe(struct int_stackframe * frame)
{
    vga_printf("    gs  : %p, fs  : %p, es  : %p, ds  : %p\n", 
            frame->gs, frame->fs, frame->es, frame->ds);

    vga_printf("    edi : %p, esi : %p, ebp : %p, esp : %p\n", 
            frame->edi, frame->esi, frame->ebp, frame->esp);

    vga_printf("    eax : %p, ebx : %p, ecx : %p, edx : %p\n", 
            frame->eax, frame->ebx, frame->ecx, frame->edx);

    vga_printf("    eip : %p, cs  : %p\n", frame->eip, frame->cs);

    vga_printf("    eflags  : %p\n", frame->eflags);
    vga_printf("    useresp : %p\n", frame->useresp);
    vga_printf("    ss      : %p\n", frame->ss);
}

void interrupts_handler(struct int_stackframe * frame)
{
    if (frame->int_no < 32)
    {
        vga_printf("%s exception - system halted...\n", 
                exception_messages[frame->int_no]);
        print_stackframe(frame);
        for (;;);
    }
    else
    {
        void (* irq_handler) (struct int_stackframe *);
        irq_handler = irq_routines[frame->int_no - 32];

        if (frame->int_no > 32)
            vga_printf("irq on int_no=%d\n", frame->int_no);
        if (irq_handler != NULL) {
            irq_handler(frame);
        }
    }

    pic_ack_int(frame->int_no);
}

void irq_install_handler(uint32_t irq, 
        void (* handler) (struct int_stackframe *))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(uint32_t irq)
{
    irq_routines[irq] = NULL;
}
