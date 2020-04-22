#include <kernel/x86/gdt.h>

#define NUM_GDT_ENTRIES 3

static struct gdt_entry gdt[NUM_GDT_ENTRIES];

static void gdt_entry_init(struct gdt_entry * entry,
        uint32_t baseaddr, uint32_t limit, uint8_t access, uint8_t flags)
{
    entry->limit_0to15 = (uint16_t) (limit & 0xFFFF);
    entry->baseaddr_0to15 = (uint16_t) (baseaddr & 0xFFFF);
    entry->baseaddr_16to23 = (uint8_t) ((baseaddr >> 16) & 0xFF);
    entry->access = access;
    entry->limit_16to19 = (limit >> 16) & 0x0F;
    entry->flags = flags;
    entry->baseaddr_24to31 = (uint8_t) ((baseaddr >> 24) & 0xFF);
}

void gdt_init()
{
    gdt_entry_init(&gdt[0], 0, 0         , 0   , 0   );
    gdt_entry_init(&gdt[1], 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_entry_init(&gdt[2], 0, 0xFFFFFFFF, 0x92, 0xCF);

    struct gdt_ptr ptr;
    ptr.length = sizeof(struct gdt_entry) * NUM_GDT_ENTRIES;
    ptr.startaddr = (uint32_t) &gdt;

    extern void gdt_flush(uint32_t);
    gdt_flush((uint32_t) &ptr);
}
