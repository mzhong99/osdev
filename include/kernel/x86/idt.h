#ifndef __IDT_H__
#define __IDT_H__

#include <stdint.h>

#define IRQ_INDEX_PIT           0
#define IRQ_INDEX_KEYBOARD      1

struct idt_ptr
{
    uint16_t length;
    uint32_t startaddr;
} __attribute__((packed));

struct idt_entry
{
    uint16_t offset_0to15;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_16to31;
} __attribute__((packed));

// variables must be listed in reverse order of pushes
struct int_stackframe
{
    // from manually pushing segment registers last
    uint32_t gs;    
    uint32_t fs; 
    uint32_t es; 
    uint32_t ds;

    // from pushad
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // from error and intcode macro push
    uint32_t int_no;
    uint32_t err_code;

    // the processor automatically pushes these
    uint32_t eip;
    uint32_t cs; 
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;

} __attribute__((packed));

void idt_init();
void irq_install_handler(uint32_t irq, 
        void (* handler) (struct int_stackframe *));

#endif
