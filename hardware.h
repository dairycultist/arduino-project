#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

void init_hardware();

void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c);

#endif