#include "headers.hpp"

class Terminal {

public:
	size_t row;
	size_t column;
	uint8_t color;
	uint16_t* buffer;

	Terminal();
	void write(const char* data);
	void write_c(const char* data, uint8_t color);
	void putchar(char c);
	void put_at(char c, uint8_t color, size_t x, size_t y);
	void set_color(uint8_t n_color);

	void putchar_c(char c, uint8_t color);
	void put_hex(uint32_t num, int remaining_digits, uint8_t color);
	void print_hex(uint32_t num, uint8_t color);

};