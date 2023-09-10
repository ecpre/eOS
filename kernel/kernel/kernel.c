#include <stdio.h>
#include <kernel/tty.h>
#include "multiboot.h"

void kernel_main(int a) {
	terminal_init();
	a++;
	printf("eOS\n");
	printf("printf tests: \n");
	printf("char: %c %c", 0x1, '\n');
	printf("str: %s\n", "this is a string!");
	printf("int: %d unsigned int: %u, unsigned underflow: %u\n", -5, 5, -5);
	printf("pointer: %p hex: %x\n", &a, 0xdeadbeef);
}
