#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int intc) {
	char c = (char) intc;
	terminal_putc(c);
	return intc;
}

int puts(const char* string) {
	int strlen = 0;
	while(*string) {
		putchar(*string);
		strlen++;
		string++;
	}
	return strlen;
}
