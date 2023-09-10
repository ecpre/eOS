#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <stdlib.h>
#include <stdint.h>
#include "multiboot.h"
#define KERNEL_VIRTUAL_LOC 0xC0000000

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

void multiboot(multiboot_info_t*, uint32_t grub_magic);

void kernel_main(uint32_t multiboot_loc, uint32_t grub_magic) {
	terminal_init();
	printf("eOS\n");
	printf("printf tests: \n");
	printf("char: %c %c", 0x1, '\n');
	printf("str: %s\n", "this is a string!");
	printf("int: %d unsigned int: %u, unsigned underflow: %u\n", -5, 5, -5);
	printf("pointer: %p hex: %x\n", multiboot_loc, 0xdeadbeef);
	printf("%p\n", multiboot_loc);
	multiboot_info_t* mbd = (multiboot_info_t*) multiboot_loc;
	printf("%p\n", mbd);
	multiboot(mbd, grub_magic);
	printf("%p\n", &_kernel_start);
	printf("%p\n", &_kernel_end);
}

void multiboot(multiboot_info_t* mbd, uint32_t grub_magic) {
	terminal_set_color(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
	printf("MULTIBOOT RAM INFORTMATION\n");
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
	printf("total mem: %llxKiB\n", mem_size/1024);
	printf("available: %llxKiB\n", mem_size/1024);

	terminal_color_reset();
}
