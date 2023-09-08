#include <stdio.h>
#include <kernel/tty.h>

void kernel_main(int a) {
	terminal_init();
	a++;
	printf("stest\nadfa\nafas");
}
