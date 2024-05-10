#ifndef ARCH_I386_GDT_H
#define ARCH_I386_GDT_H 1

#include <stdint.h>

typedef struct GDT_entry {
	uint32_t limit_low		: 16;
	uint32_t base_low		: 24;
	uint32_t accessed		: 1;
	uint32_t rw			: 1;
	uint32_t conform_down		: 1;
	uint32_t code			: 1;
	uint32_t code_data_segment	: 1;
	uint32_t DPL			: 2; // privilege
	uint32_t present		: 1;
	uint32_t limit_high		: 4;
	uint32_t available		: 1;
	uint32_t long_mode		: 1;
	uint32_t big			: 1;
	uint32_t granularity		: 1;
	uint32_t base_high		: 8;
} __attribute__((packed)) GDT_entry;

typedef struct {
	uint16_t limit;
	GDT_entry* gdt_ptr;
} __attribute__((packed)) GDT_descriptor;

void x86_32_GDT_init();

#endif
