#include <kernel/gdt.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern void __attribute__((cdecl)) x86_32_GDT_load(GDT_descriptor* descriptor, uint16_t code, uint16_t data);

void x86_32_GDT_init() {
	GDT_entry gdt[5];		// change to 6 for tss
	GDT_entry *gdt_null = &gdt[0];
	GDT_entry *gdt_ring0_code = &gdt[1];
	GDT_entry *gdt_ring0_data = &gdt[2];
	GDT_entry *gdt_ring3_code = &gdt[3];
	GDT_entry *gdt_ring3_data = &gdt[4];
	GDT_entry *gdt_tss = &gdt[5];

	memset(gdt_null, 0, 8);
	memset(gdt_tss, 0, 8);

	gdt_ring0_code->limit_low = 0xFFFF;
	gdt_ring0_code->base_low=0;
	gdt_ring0_code->accessed=0;
	gdt_ring0_code->rw=1;
	gdt_ring0_code->conform_down=0;
	gdt_ring0_code->code=1;
	gdt_ring0_code->code_data_segment=1;
	gdt_ring0_code->DPL=0;
	gdt_ring0_code->present=1;
	gdt_ring0_code->limit_high=0xF;
	gdt_ring0_code->available=1;
	gdt_ring0_code->long_mode=0;
	gdt_ring0_code->big=1;
	gdt_ring0_code->granularity=1;
	gdt_ring0_code->base_high=0;

	*gdt_ring0_data = *gdt_ring0_code;
	*gdt_ring3_code = *gdt_ring0_code;
	*gdt_ring3_data = *gdt_ring0_code;

	gdt_ring0_data->code = 0;
	gdt_ring3_code->DPL=3;
	gdt_ring3_data->DPL=3;
	gdt_ring3_data->code = 0;
	
	GDT_descriptor gdt_desc = {sizeof(gdt)-1, gdt};
	
	x86_32_GDT_load(&gdt_desc, 0x10, 0x08);
}
