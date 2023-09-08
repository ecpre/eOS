#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_init();
void terminal_set_color(uint8_t);
void terminal_putc(char);

#endif
