#include <lib/bitmap.h>

void set_bitmap(bitmap* bmap, uint32_t loc, uint8_t val) {
	uint32_t addr = loc / 8;
	uint8_t addr_offset = loc % 8;
	if (val)
		bmap -> bytes[addr] |= 1 << addr_offset;
	else
		bmap -> bytes[addr] &= ~(1 << addr_offset);
}

uint8_t get_bitmap(bitmap* bmap, uint8_t loc) {
	uint32_t addr = loc / 8;
	uint8_t addr_offset = loc % 8;
	return ((bmap -> bytes[addr]) >> addr_offset) & 1;
}
