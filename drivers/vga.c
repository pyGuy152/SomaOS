#include "vga.h"

/* 80x25 text mode: two bytes per cell, low = ASCII, high = bg << 4 | fg. */
#define VGA_MEM    ((volatile uint16_t *)0xB8000)
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

static size_t  row, col;
static uint8_t color = 0x0F;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

void vga_set_color(uint8_t attr)
{
	color = attr;
}

void vga_clear(void)
{
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		VGA_MEM[i] = (uint16_t)' ' | (uint16_t)color << 8;
	row = col = 0;
}

void vga_put(char c)
{
	if (c == '\n') {
		col = 0;
		if (++row == VGA_HEIGHT) row = 0;
		return;
	}
	VGA_MEM[row * VGA_WIDTH + col] = (uint16_t)c | (uint16_t)color << 8;
	if (++col == VGA_WIDTH) {
		col = 0;
		if (++row == VGA_HEIGHT) row = 0;
	}
}

void vga_write(const char *s)
{
	for (size_t i = 0; s[i]; i++)
		vga_put(s[i]);
}

void vga_write_at(int r, const char *s, uint8_t attr)
{
	volatile uint16_t *cell = VGA_MEM + r * VGA_WIDTH;
	int i = 0;

	while (s[i] && i < VGA_WIDTH) {
		cell[i] = (uint16_t)s[i] | (uint16_t)attr << 8;
		i++;
	}
	while (i < VGA_WIDTH) {
		cell[i] = (uint16_t)' ' | (uint16_t)attr << 8;
		i++;
	}
}

void vga_write_hex_at(int r, int c, uint32_t val, uint8_t attr)
{
	volatile uint16_t *cell = VGA_MEM + r * VGA_WIDTH + c;
	const char *digits = "0123456789ABCDEF";

	for (int i = 0; i < 8; i++)
		cell[i] = (uint16_t)digits[(val >> ((7 - i) * 4)) & 0xF] | (uint16_t)attr << 8;
}
