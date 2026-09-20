#include "gdt.h"
#include "idt.h"
#include "vga.h"

void kernel_main(void)
{
	vga_set_color(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
	vga_clear();

	vga_write("Hello, World!\n");
	vga_write("Welcome to SomaOS.\n\n");

	gdt_init();
	vga_write("[ OK ] GDT loaded  (flat model, 5 entries)\n");

	idt_init();
	vga_write("[ OK ] IDT loaded  (256 vectors, isr0 installed)\n");

	vga_write("\nKernel ready.\n");

	for (;;)
		__asm__ volatile ("hlt");
}
