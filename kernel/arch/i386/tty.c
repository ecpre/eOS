#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/vga.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_y;
size_t terminal_x;
uint8_t terminal_color;
uint16_t* terminal_buf;

void terminal_init() {
	terminal_y = 0;
	terminal_x = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buf = (uint16_t*) 0xC03FF000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = (y*VGA_WIDTH) + x;
			terminal_buf[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_set_color(uint8_t color) {
	terminal_color = color;
}

void terminal_color_reset() {
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR, BLACK);
}

void terminal_put_entry(char c, uint8_t color, size_t x, size_t y) {
	size_t index = y * VGA_WIDTH + x;
	unsigned char uc = (unsigned char) c;
	if (uc == '\n') {
		terminal_y++;
		// i am not sure why this needs to be -1. 
		terminal_x = -1;
	}
	else {
		terminal_buf[index] = vga_entry(uc, color);
	}
}

void terminal_putc(char c) {
	unsigned char uc = (unsigned char) c;
	terminal_put_entry(uc, terminal_color, terminal_x, terminal_y);
	if (++terminal_x == VGA_WIDTH) {
		terminal_x = 0;
		if (++terminal_y == VGA_HEIGHT) {
			terminal_y = 0;
		}
	}
}
