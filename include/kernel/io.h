#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

extern void io_sendbyte(uint16_t port, uint8_t data);
extern uint8_t io_recvbyte(uint16_t port);
extern void io_wait();

#endif
