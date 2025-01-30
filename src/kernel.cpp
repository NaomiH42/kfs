#include "headers.hpp"
#include "vga.hpp"
#include "Terminal.hpp"

// uint16_t limit;
// 	uint16_t base_l;
// 	uint8_t	base_m;
// 	uint8_t access_byte;
// 	uint8_t flags;
// 	uint8_t base_h;

void print_entry(Terminal terminal, struct GDTEntry gdt_entry) {
	uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
	terminal.write("      ");
	terminal.print_hex(gdt_entry.limit, color);
	terminal.write(" ");
	terminal.print_hex(gdt_entry.base_l, color);
	terminal.write(" ");
	terminal.print_hex(gdt_entry.base_m, color);
	terminal.write(" ");
	terminal.print_hex(gdt_entry.access_byte, color); // gdt_entry.access_byte & 0xF to remove the accessed bit set by gpu
	terminal.write(" ");
	terminal.print_hex(gdt_entry.flags, color);
	terminal.write(" ");
	terminal.print_hex(gdt_entry.base_h, color);
}

void printk(Terminal terminal) {
	struct GDTPointer *gdt_ptr = (struct GDTPointer*)0x00000800;
    struct GDTEntry *gdt_entires = (struct GDTEntry*)gdt_ptr->base;

	terminal.write_c("         LIMIT     BASE_L     BASE_M     ACCESS     FLAGS      BASE_H\n", VGA_COLOR_RED);
	terminal.write_c("   -------------------------------------------------------------------------\n", VGA_COLOR_RED);

	terminal.write("Null segment:\n");
	print_entry(terminal, gdt_entires[0]);
	terminal.write("\nKernel code:\n");
	print_entry(terminal, gdt_entires[1]);
	terminal.write("\nKernel data:\n");
	print_entry(terminal, gdt_entires[2]);
	terminal.write("\nKernel stack:\n");
	print_entry(terminal, gdt_entires[3]);

	terminal.write("\nUser code:\n");
	print_entry(terminal, gdt_entires[4]);
	terminal.write("\nUser data:\n");
	print_entry(terminal, gdt_entires[5]);
	terminal.write("\nUser stack:\n");
	print_entry(terminal, gdt_entires[6]);

}

extern "C" void kernel_main() {
	init_gdt();

	Terminal terminal = Terminal();


	// terminal.write("42\n");
	terminal.write_c("       :::      ::::::::   :::    :::  ::::::::: ::::::::\n", VGA_COLOR_LIGHT_MAGENTA);
	terminal.write_c("      :+:     :+:     :+:  :+:   :+:  :+:        :+:    :+:\n", VGA_COLOR_LIGHT_MAGENTA);
	terminal.write_c("    +:+ +:+         +:+    +:+  +:+   +:+        +:+ \n", VGA_COLOR_LIGHT_MAGENTA);
	terminal.write_c("  +#+  +:+       +#+       +#++:++    +#++:++#:  +#++:++#++\n", VGA_COLOR_LIGHT_MAGENTA);
	terminal.write_c("+#+#+#+#+#+   +#+          +#+  +#+   +#+               +#+\n", VGA_COLOR_LIGHT_MAGENTA);
	terminal.write_c("     #+#    #+#            #+#   #+#  #+#        #+#    #+#\n", VGA_COLOR_LIGHT_MAGENTA);
	terminal.write_c("    ###   ###########      ###    ### ###        ########\n", VGA_COLOR_LIGHT_MAGENTA);
	terminal.write("\n\n");
	
	
	printk(terminal);

}
