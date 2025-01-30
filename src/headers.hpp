#ifndef HEADERS_HPP
 #define HEADERS_HPP

#include <cstdbool>
#include <cstddef>
#include <cstdint>

struct GDTPointer {
    uint16_t limit;    // Size of the GDT in bytes minus one
    uint32_t base;     // Base address of the GDT
} __attribute__((packed));  // Prevent padding

struct GDTEntry {
	uint16_t limit;
	uint16_t base_l;
	uint8_t	base_m;
	uint8_t access_byte;
	uint8_t flags;
	uint8_t base_h;
} __attribute__((packed));


void init_gdt();
void set_gdt_entry(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

size_t strlen(const char* str);


#endif