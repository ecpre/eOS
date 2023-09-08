#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
	printf("kernel panic!\n");
	asm volatile("hlt");
#else
	printf("abort()\n");
	// TODO: actually abort
#endif
	while(1) {}
	__builtin_unreachable();
}
