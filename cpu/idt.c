#include "idt.h"
#include "gdt.h"
#include "vga.h"
#include "io.h"

#define PANIC_ATTR 0x4F   /* white on red */

void fault_handler(struct registers *r)
{
	vga_write_at(0, " *** SomaOS FAULT: Divide-By-Zero (vector 0) ***", PANIC_ATTR);
	vga_write_at(1, " EIP:          EAX:          EFLAGS:", PANIC_ATTR);
	vga_write_hex_at(1, 6,  r->eip,    PANIC_ATTR);
	vga_write_hex_at(1, 20, r->eax,    PANIC_ATTR);
	vga_write_hex_at(1, 37, r->eflags, PANIC_ATTR);
	vga_write_at(2, " System halted.", PANIC_ATTR);

	/* Returning would re-execute the faulting instruction. */
	for (;;)
		__asm__ volatile ("cli; hlt");
}

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr   ip;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt[num].base_low  = (uint16_t)(base & 0xFFFF);
	idt[num].base_high = (uint16_t)((base >> 16) & 0xFFFF);
	idt[num].sel       = sel;
	idt[num].always0   = 0;
	idt[num].flags     = flags;
}

extern void isr0(void);
extern void irq1(void);

void idt_init(void)
{
	ip.limit = (uint16_t)(sizeof(struct idt_entry) * IDT_ENTRIES - 1);
	ip.base  = (uint32_t)&idt;

	/* Zeroed gates have P=0, so an unhandled vector raises #GP*/
	for (int i = 0; i < IDT_ENTRIES; i++)
		idt_set_gate((uint8_t)i, 0, 0, 0);

	idt_set_gate(0, (uint32_t)isr0, GDT_KERNEL_CODE_SEL, IDT_FLAG_KERNEL_INT);
	idt_set_gate(33, (uint32_t)irq1, GDT_KERNEL_CODE_SEL, IDT_FLAG_KERNEL_INT);

	idt_load(&ip);

	outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28);
    outb(0x21, 0x04); outb(0xA0, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0xFD); outb(0xA1, 0xFF);

	__asm__ volatile ("sti");
}
