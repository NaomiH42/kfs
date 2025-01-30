#include "headers.hpp"

// #define GDT_PRESENT        0x80
// #define GDT_RING0         0x00
// #define GDT_RING3         0x60
// #define GDT_TYPE_CODE     0x1A    // Code, readable, non-conforming
// #define GDT_TYPE_DATA     0x12    // Data, writable, expand-up
// #define GDT_TYPE_STACK    0x12    // Stack segment (same as data but grows down)

// // GDT granularity byte flags
// #define GDT_GRANULARITY_4K 0x80   // 4K granularity
// #define GDT_PROTECTED_MODE 0x40    // 32-bit protected mode
// #define GDT_LIMIT_HIGH     0x0F    // High 4 bits of limit

extern "C" void gdt_flush(unsigned int);

#define GDT_NUM 7
struct GDTEntry gdt_entries[GDT_NUM];
struct GDTPointer *gdt_ptr = (GDTPointer*)0x00000800;



void init_gdt() {
    gdt_ptr->limit = (sizeof(GDTEntry) * GDT_NUM) - 1;
    gdt_ptr->base = reinterpret_cast<uint32_t>(&gdt_entries);

    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // kernel code. 0x9A - flags, limit + gran set 4 KiB page
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // kernel data. 0x9A - flags, limit + gran set 4 KiB page
    set_gdt_entry(3, 0, 0xFFFFFFFF, 0x92, 0xCF); // kernel stack - using data
    set_gdt_entry(4, 0, 0xFFFFFFFF, 0xFA, 0xCF); // user code. 0x9A - flags, limit + gran set 4 KiB page
    set_gdt_entry(5, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user data. 0x9A - flags, limit + gran set 4 KiB page
    set_gdt_entry(6, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user stack - using data


    gdt_flush(reinterpret_cast<uint32_t>(gdt_ptr));
}

void set_gdt_entry(uint32_t num, uint32_t base, uint32_t limit, 
    uint8_t access, uint8_t gran) {
        gdt_entries[num].base_l = (base & 0xFFFF); // gives us the lower set of bits
        gdt_entries[num].base_m = (base >> 16) & 0xFF; // shifts 16 to the right and takes bottom 8 bits?
        gdt_entries[num].base_h = (base >> 24) & 0xFF;

        gdt_entries[num].limit = (limit & 0xFFFF); // limit split into two pieces
        gdt_entries[num].flags = (limit >> 16) & 0x0F; // limits and flags next to each other. We place limit first and place flags on top of it
        gdt_entries[num].flags |= (gran & 0xF0); // split it half, because we don't have 4 bit integer
    
        gdt_entries[num].access_byte = access;
    }
