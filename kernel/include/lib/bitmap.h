#ifndef LIB_BITMAP_H
#define LIB_BITMAP_H

#include <stdint.h>

#define USED 1
#define FREE 0

typedef struct bitmap {
	// just make the program keep track of the bitmap size on their own?
	// i'll make it like that for now and reevaluate in the future
	//uint32_t size;
	uint8_t* bytes;
} bitmap;

bitmap* create_bitmap(uint32_t);
void bitmap_initialize();
void set_bitmap(bitmap*, uint32_t, uint8_t);
uint8_t get_bitmap(bitmap*, uint32_t);

#endif
