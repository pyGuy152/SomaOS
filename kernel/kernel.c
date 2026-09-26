#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "pit.h"
#include "kprintf.h"
#include "string.h"
#include "pmm.h"

void kernel_main(multiboot_info_t *mboot_info)
{
	vga_set_color(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
	vga_clear();

	vga_write("Welcome to SomaOS.\n\n");

	gdt_init();
	vga_write("GDT loaded\n");

	idt_init();
	vga_write("IDT loaded (isr0, irq0, irq1)\n");

	pit_init(100);
	kprintf("Initialized PIT at target frequency of %d\n",100);

	pmm_init(mboot_info);
	kprintf("Initialized PMM\n Total memory: %dMB\n",pmm_get_total_frames()/256);

	vga_write("\nKernel ready. Type \"help\" and press enter to see all commands\n");

	for (;;)
		__asm__ volatile ("hlt");
}
