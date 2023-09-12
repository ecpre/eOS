#ifndef LIB_BITMAP_H
#define LIB_BITMAP_H

#include <stdint.h>

typedef struct bitmap {
	uint32_t size;
	uint8_t* bytes;
} bitmap;

bitmap* create_bitmap(uint32_t);
void set_bitmap(bitmap*, uint32_t, uint8_t);
uint8_t get_bitmap(bitmap*, uint8_t);

#endif
