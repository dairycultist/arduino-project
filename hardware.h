#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

void init_hardware();

void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b);

uint16_t get_x1024();
uint16_t get_y1024();

void sleep();

#endif