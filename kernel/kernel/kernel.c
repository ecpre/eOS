#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <stdlib.h>
#include <stdint.h>
#include <lib/bitmap.h>
#include <kernel/page.h>

#include "multiboot.h"

#define KERNEL_VIRTUAL_LOC 0xC0000000

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;
extern uint32_t _mmap_start;
extern uint32_t _mmap_end;

void multiboot(multiboot_info_t*, uint32_t grub_magic);

void kernel_main(uint32_t multiboot_loc, uint32_t grub_magic) {
	terminal_init();
	printf("eOS\n");
	printf("printf tests: \n");
	printf("char: %c %c", 0x1, '\n');
	printf("str: %s\n", "this is a string!");
	printf("int: %d unsigned int: %u, unsigned underflow: %u\n", -5, 5, -5);
	printf("pointer: %p hex: %x\n", multiboot_loc, 0xdeadbeef);
	multiboot_info_t* mbd = (multiboot_info_t*) multiboot_loc;
	multiboot(mbd, grub_magic);
	
	printf("creating bitmap at loc: %p\n", &_mmap_start);
	bitmap bmap;
	bmap.size = 0xffe*8;
	uint8_t* bbytes = (void*) &_mmap_start;
	bmap.bytes = bbytes;
	set_bitmap(&bmap, 1, 1);
	
	page_directory_t* default_pd;
	asm volatile("movl %%cr3, %0" : "=r"(default_pd):);
	default_pd = (void*) default_pd + KERNEL_VIRTUAL_LOC;
	printf("%x\n", default_pd);
	printf("%x\n", (default_pd->tables[768].addr));
	printf("%x\n", &_mmap_end);
	uint32_t newpd_loc = (uint32_t) &_mmap_end;
	newpd_loc -= KERNEL_VIRTUAL_LOC;
	paging_reinit(default_pd, newpd_loc);
	page_directory_t* new_pd;
	asm volatile("movl %%cr3, %0" : "=r"(new_pd):);
	printf("Recursive paging pd: %x\n", new_pd);
	new_pd = (page_directory_t*) 0xfffff000;
	printf("%x\n", new_pd->tables[769].addr);
}

void multiboot(multiboot_info_t* mbd, uint32_t grub_magic) {
	terminal_set_color(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
	printf("MULTIBOOT RAM INFORMATION\n");
	printf("%x\n", mbd);
	if (grub_magic != 0x2BADB002) {
		printf("BAD MAGIC NUM!\n");
		abort();
	}
	if (!(mbd->flags >> 6 & 0x1)) {
		printf("BAD MEMORY MAP!\n");
		abort();
	}
	
	uint64_t mem_size = 0;
	uint64_t available = 0;

	for (uint32_t i = 0; i<mbd->mmap_length; i+=sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)
			(mbd->mmap_addr + i + KERNEL_VIRTUAL_LOC);
		printf("MAP INDEX: %d | START: %llx | LEN: %llx | TYPE: ",
				i, mmmt->addr, mmmt->len);

		mem_size += mmmt->len;

		switch(mmmt->type) {
		case MULTIBOOT_MEMORY_AVAILABLE:
			printf("AVAILABLE\n");
			available += mmmt-> len;
			break;
		case MULTIBOOT_MEMORY_RESERVED:
			printf("RESERVED\n");
			break;
		case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
			printf("ACPI RECLAIMABLE\n");
			break;
		case MULTIBOOT_MEMORY_NVS:
			printf("ACPI NVS\n");
			break;
		case MULTIBOOT_MEMORY_BADRAM:
			printf("BAD\n");
			break;
		default:
			printf("BAD MEMORY MAP!\n");
			abort();
			break;

		}
	}
	printf("total mem: %llx KiB\n", mem_size/1024);
	printf("available: %llx KiB\n", available/1024);
	printf("necessary bitmap size: %x bytes\n", (mem_size >> 15));

	terminal_color_reset();
}
