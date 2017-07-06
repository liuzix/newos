//
// Created by Zixiong Liu on 12/16/16.
//

#include "vga.h"
#include "x86.h"
#include "lib/assert.h"

int vga::pos_x = 0;
int vga::pos_y = 0;

inline uint8_t make_color(vga::vga_color_t fg, vga::vga_color_t bg)
{
	return fg | bg << 4;
}

inline uint16_t make_vgaentry(char c, vga::vga_color_t color)
{
	uint16_t c16 = (uint16_t)c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

void update_cursor(int row, int col)
{

	unsigned short position = (row * 80) + col;

	// cursor LOW port to vga INDEX register
	x86::outb(0x3D4, 0x0F);
	x86::outb(0x3D5, (unsigned char)(position & 0xFF));
	// cursor HIGH port to vga INDEX register
	x86::outb(0x3D4, 0x0E);
	x86::outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

void vga::vga_init() {
	pos_x = 0;
	for (int i = 0; i < buffer_height * buffer_width; i++) {
		terminal_buffer[i] = 0;
	}
}

void vga::putc(const char c, vga::vga_color_t fg_color) {
	//update_cursor(pos_y, pos_x);
	if (c == '\n') {
		new_line();
		return;
	}

	uint8_t color = make_color(fg_color, COLOR_BLACK);
	terminal_buffer[pos_x + pos_y*buffer_width] = make_vgaentry(c, color);

	pos_x++;
	if (pos_x == buffer_width) {
		pos_x = 0;
		pos_y++;
	}

	if (pos_y == buffer_height) {
		pos_y = buffer_height - 1;
		scroll();
	}
}

inline void vga::new_line() {
	pos_y++;
	pos_x = 0;
	if (pos_y == buffer_height) {
		pos_y = buffer_height - 1;
		scroll();
	}
	update_cursor(pos_y, pos_x);

}

inline void vga::scroll() {
	for (int j = 1; j < buffer_height; j++) {
		for (int i = 0; i < buffer_width; i++) {
			terminal_buffer[i + (j - 1) * buffer_width]
				= terminal_buffer[i + j * buffer_width];
		}
	}

	for (int i = 0; i < buffer_width; i++) {
		terminal_buffer[i + (buffer_height - 1) * buffer_width]
			= 0; // empty the last row
	}
};

void vga::print(const char *str, vga::vga_color_t color) {
	char c;
	while ((c = *str) != '\0') {
		str++;
		/*
		if (c == '\n') {
		  new_line();
		  continue;
		}
		 */
		putc(c, color);
	}
}

void vga::print(const char *str) {
	print(str, COLOR_WHITE);
}

