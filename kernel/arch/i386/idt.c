#include <kernel/idt.h>

IDT_entry idt[256];

IDT_descriptor idt_desc = {sizeof(idt)-1, &idt};

void x86_32_IDT_set_gate(int interrupt, void* base, uint16_t segment_desc, uint8_t flags) {
	idt[interrupt].base_low = ((uint32_t) base) & 0xffff;
	idt[interrupt].segment_selector = segment_desc;
	idt[interrupt].reserved = 0;
	idt[interrupt].flags = flags;
	idt[interrupt].base_high = ((uint32_t) base >> 16) & 0xffff;
}

void x86_32_IDT_enable_gate(int interrupt) {
	idt[interrupt].flags |= (IDT_FLAG_PRESENT);
}

void x86_32_IDT_disable_gate(int interrupt) {
	idt[interrupt].flags &= ~(IDT_FLAG_PRESENT);
}

void x86_32_IDT_init() {
	

	x86_32_IDT_load();
}
