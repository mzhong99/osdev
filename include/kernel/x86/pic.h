#ifndef __PIC_H__
#define __PIC_H__

#include <stdint.h>

void pic_ack_int(uint8_t irq);
void pic_remap();

#endif

