#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>
#include <stdbool.h>

struct software_timer
{
    uint32_t start_ticks;
    uint32_t wait_ticks;
};

void timers_init();

struct software_timer software_timer_construct_ms(uint32_t ms);
struct software_timer software_timer_construct_ticks(uint32_t ticks);

void software_timer_start(struct software_timer * timer);
bool software_timer_expired(struct software_timer * timer);

#endif
