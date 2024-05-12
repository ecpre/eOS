#ifndef ARCH_I386_IDT_H
#define ARCH_I386_IDT_H 1

#include <stdint.h>

typedef struct IDT_entry {
	uint16_t base_low;
	uint16_t segment_selector;
	uint8_t reserved;
	uint8_t flags;
	uint16_t base_high;
} __attribute__((packed)) IDT_entry;

typedef struct IDT_descriptor {
	uint16_t limit;
	IDT_entry* idt_ptr;
} __attribute__((packed)) IDT_descriptor;

typedef enum IDT_FLAGS {
	IDT_FLAG_GATE_TASK = 0x5,
	IDT_FLAG_GATE_16BIT_INT = 0x6,
	IDT_FLAG_GATE_16BIT_TRAP = 0x7,
	IDT_FLAG_GATE_32BIT_INT = 0xE,
	IDT_FLAG_GATE_32BIT_TRAP = 0xF,

	IDT_FLAG_RING0 = (0 << 5),
	IDT_FLAG_RING1 = (1 << 5),
	IDT_FLAG_RING2 = (2 << 5),
	IDT_FLAG_RING3 = (3 << 5),

	IDT_FLAG_PRESENT = 0x80,
} IDT_FLAGS;

void __attribute__((cdecl)) x86_32_IDT_load();
void x86_32_IDT_init();
void x86_32_IDT_set_gate(int interrupt, void* base, uint16_t segement_desc, uint8_t flags);
void x86_32_IDT_enable_gate(int interrupt);
void x86_32_IDT_disable_date(int interrupt);

#endif
