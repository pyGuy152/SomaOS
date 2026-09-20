#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_ENTRIES 256

/* present | DPL | 32-bit gate type */
#define IDT_FLAG_KERNEL_INT  0x8E
#define IDT_FLAG_KERNEL_TRAP 0x8F
#define IDT_FLAG_USER_INT    0xEE

struct idt_entry {
	uint16_t base_low;
	uint16_t sel;
	uint8_t  always0;
	uint8_t  flags;
	uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

/*
 * Saved CPU state, in memory order. Matches the stubs in cpu/idt_load.s that
 * push no error code; vectors 8, 10-14 and 17 get one from the CPU.
 */
struct registers {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t eip, cs, eflags;
};

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_init(void);

/* cpu/idt_load.s */
extern void idt_load(struct idt_ptr *ip);
extern void isr0(void);

#endif
