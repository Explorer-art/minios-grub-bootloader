#ifndef PORT_H
#define PORT_H

#include <stdint.h>

extern uint8_t read_port(uint16_t port);
extern void write_port(uint16_t port, uint8_t value);

#endif