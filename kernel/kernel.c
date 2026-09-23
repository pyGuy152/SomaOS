#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "pit.h"
#include "string.h"

void kernel_main(void)
{
	vga_set_color(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
	vga_clear();

	vga_write("Welcome to SomaOS.\n\n");

	gdt_init();
	vga_write("GDT loaded\n");

	idt_init();
	vga_write("IDT loaded (isr0, irq0, irq1)\n");

	pit_init(100);
	
	vga_write("Initialized PIT\n");

	String ready_message = str_make("\nKernel ready.\n"); // checking if it works! 
	vga_write(ready_message.data);

	for (;;)
		__asm__ volatile ("hlt");
}
