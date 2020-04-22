#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>

struct gdt_ptr
{
    uint16_t length;
    uint32_t startaddr;
} __attribute__((packed));

struct gdt_entry
{
    uint16_t limit_0to15;
    uint16_t baseaddr_0to15;
    uint8_t baseaddr_16to23;
    uint8_t access;
    uint8_t limit_16to19 : 4;
    uint8_t flags : 4;
    uint8_t baseaddr_24to31;
} __attribute__((packed));

void gdt_init();

#endif
