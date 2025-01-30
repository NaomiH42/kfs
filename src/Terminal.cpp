#include "Terminal.hpp"
#include "vga.hpp"

Terminal::Terminal() {
	row = 0;
	column = 0;
	color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			buffer[index] = vga_entry(' ', color);
		}
	}
}

void Terminal::write(const char* data) {
	size_t size = strlen(data);

	for (size_t i = 0; i < size; i++)
		putchar(data[i]);
}

void Terminal::write_c(const char* data, uint8_t color) {
	size_t size = strlen(data);

	for (size_t i = 0; i < size; i++)
		putchar_c(data[i], color);
}

void Terminal::putchar(char c) {
	if (c == '\n') {
		row++;
		column = 0;
		return;
	}
	put_at(c, color, column, row);
	if (++column == VGA_WIDTH) {
		column = 0;
		if (++row == VGA_HEIGHT)
			row = 0;
	}
}


void Terminal::putchar_c(char c, uint8_t color) {
	if (c == '\n') {
		row++;
		column = 0;
		return;
	}
	put_at(c, color, column, row);
	if (++column == VGA_WIDTH) {
		column = 0;
		if (++row == VGA_HEIGHT)
			row = 0;
	}
}

void Terminal::put_at(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	buffer[index] = vga_entry(c, color);
}

void Terminal::put_hex(uint32_t num, int remaining_digits, uint8_t color) {
    char hex_digits[] = "0123456789ABCDEF";

    if (remaining_digits == 0) return; 
    put_hex(num / 16, remaining_digits - 1, color);
    putchar_c(hex_digits[num % 16], color); 
}

void Terminal::print_hex(uint32_t num, uint8_t color) {
	putchar_c('0', color);
	putchar_c('x', color);
	put_hex(num, 8, color);
}


void Terminal::set_color(uint8_t n_color) {
	color = n_color;
}
