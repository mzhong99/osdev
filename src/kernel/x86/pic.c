#include <kernel/x86/pic.h>
#include <kernel/io.h>

#define PIC1            0x20        // io base address for master PIC
#define PIC2            0xA0        // io base address for slave PIC

#define PIC1_CMD        PIC1
#define PIC1_DATA       (PIC1 + 1)
#define PIC1_OFFSET     0x20

#define PIC2_CMD        PIC2
#define PIC2_DATA       (PIC2 + 1)
#define PIC2_OFFSET     0x28

#define PIC_ACK_INT     0x20

#define ICW1_ICW4       0x01
#define ICW1_INIT       0x10
#define ICW4_8086       0x01

void pic_ack_int(uint8_t irq)
{
    if (irq > 40) {
        io_sendbyte(PIC2_CMD, PIC_ACK_INT);
    }

    io_sendbyte(PIC1_CMD, PIC_ACK_INT);
}

void pic_remap()
{
    // start initialization in cascade mode
    io_sendbyte(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    io_sendbyte(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // remap offsets
    io_sendbyte(PIC1_DATA, PIC1_OFFSET);
    io_wait();
    io_sendbyte(PIC2_DATA, PIC2_OFFSET);
    io_wait();

    // tell which one is master and which one is slave
    io_sendbyte(PIC1_DATA, 1 << 2); // tell master a slave is at IRQ2 (00000100)
    io_wait();
    io_sendbyte(PIC2_DATA, 1 << 1); // tell slave its cascade identity
    io_wait();

    // request 8086 mode on each PIC
    io_sendbyte(PIC1_DATA, ICW4_8086);
    io_wait();
    io_sendbyte(PIC2_DATA, ICW4_8086);
    io_wait();

    // zero out masks
    io_sendbyte(PIC1_DATA, 0x00);
    io_wait();
    io_sendbyte(PIC2_DATA, 0x00);
    io_wait();
}
