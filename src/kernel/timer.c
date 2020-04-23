#include <kernel/timer.h>
#include <kernel/x86/idt.h>
#include <kernel/vga.h>
#include <kernel/io.h>

#define TIMER_OSC_FREQ  1193182
#define TICK_RATE       120

#define MS_TO_TICKS(ms)                 (ms * TICK_RATE / 1000);
#define LOAD_VALUE                      ((uint16_t) (TIMER_OSC_FREQ / TICK_RATE))

#define CHANNEL_0                       0x40
#define CHANNEL_1                       0x41
#define CHANNEL_2                       0x42
#define COMMAND_REGISTER                0x43

#define CMD_COUNT_MODE_BINARY           0x00
#define CMD_COUNT_MODE_BCD              0x01

#define CMD_OPERATING_MODE_0            (0 << 1)
#define CMD_OPERATING_MODE_1            (1 << 1)
#define CMD_OPERATING_MODE_2            (2 << 1)
#define CMD_OPERATING_MODE_3            (3 << 1)
#define CMD_OPERATING_MODE_4            (4 << 1)
#define CMD_OPERATING_MODE_5            (5 << 1)

#define CMD_ACCESS_MODE_LATCH           (0 << 4)
#define CMD_ACCESS_MODE_LOWBYTE         (1 << 4)
#define CMD_ACCESS_MODE_HIGHBYTE        (2 << 4)
#define CMD_ACCESS_MODE_FULLCOUNT       (3 << 4)

#define CMD_CHANNEL_SELECT_0            (0 << 6)
#define CMD_CHANNEL_SELECT_1            (1 << 6)
#define CMD_CHANNEL_SELECT_2            (2 << 6)
#define CMD_CHANNEL_SELECT_READBACK     (3 << 6)

static uint32_t tick_count = 0;
static void timer_callback(struct int_stackframe * frame) 
{ 
    (void) frame;
    tick_count++; 
}

void timers_init()
{
    io_sendbyte(COMMAND_REGISTER, 
        CMD_COUNT_MODE_BINARY 
            | CMD_OPERATING_MODE_3 
            | CMD_ACCESS_MODE_FULLCOUNT 
            | CMD_CHANNEL_SELECT_0);

    io_sendbyte(CHANNEL_0, LOAD_VALUE & 0xFF);
    io_sendbyte(CHANNEL_0, (LOAD_VALUE >> 8) & 0xFF);

    irq_install_handler(IRQ_INDEX_PIT, timer_callback);
}

struct software_timer software_timer_construct_ms(uint32_t ms)
{
    struct software_timer timer;

    timer.start_ticks = tick_count;
    timer.wait_ticks = MS_TO_TICKS(ms);

    return timer;
}

void software_timer_start(struct software_timer * timer)
{
    timer->start_ticks = tick_count;
}

bool software_timer_expired(struct software_timer * timer)
{
    return tick_count > timer->start_ticks + timer->wait_ticks;
}
