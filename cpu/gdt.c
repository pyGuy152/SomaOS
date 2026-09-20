#include "gdt.h"

static struct gdt_entry gdt[GDT_ENTRIES];
static struct gdt_ptr   gp;

static void gdt_set_gate(int num, uint32_t base, uint32_t limit,
                         uint8_t access, uint8_t gran)
{
	gdt[num].base_low    = (uint16_t)(base & 0xFFFF);
	gdt[num].base_middle = (uint8_t)((base >> 16) & 0xFF);
	gdt[num].base_high   = (uint8_t)((base >> 24) & 0xFF);

	gdt[num].limit_low   = (uint16_t)(limit & 0xFFFF);
	gdt[num].granularity = (uint8_t)((limit >> 16) & 0x0F);
	gdt[num].granularity |= (uint8_t)(gran & 0xF0);

	gdt[num].access      = access;
}

/*
 * Flat model: every segment spans 0 - 4 GiB, so segmentation effectively
 * disappears and protection comes from paging later.
 *
 * access      0x9A/0x92 = ring-0 code/data, 0xFA/0xF2 = ring-3 code/data
 * granularity 0xCF       = 4 KiB pages, 32-bit, limit 0xFFFFF
 */
void gdt_init(void)
{
	gp.limit = (uint16_t)(sizeof(struct gdt_entry) * GDT_ENTRIES - 1);
	gp.base  = (uint32_t)&gdt;

	gdt_set_gate(0, 0, 0,          0,    0);      /* null, required */
	gdt_set_gate(1, 0, 0x000FFFFF, 0x9A, 0xCF);   /* 0x08 kernel code */
	gdt_set_gate(2, 0, 0x000FFFFF, 0x92, 0xCF);   /* 0x10 kernel data */
	gdt_set_gate(3, 0, 0x000FFFFF, 0xFA, 0xCF);   /* 0x18 user code   */
	gdt_set_gate(4, 0, 0x000FFFFF, 0xF2, 0xCF);   /* 0x20 user data   */

	gdt_flush(&gp);
}
