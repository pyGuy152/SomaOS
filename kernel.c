#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* s) {
	size_t n = 0;
	while (s[n]) n++;
	return n;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA = (uint16_t*) 0xB8000;

// Color!
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2, 
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

size_t row, col;
uint8_t color;

void put(char c) {
	if (c == '\n') { col = 0; if (++row == VGA_HEIGHT) row = 0; return; }
	VGA[row * VGA_WIDTH + col] = (uint16_t) c | (uint16_t) color << 8;
	if (++col == VGA_WIDTH) { col = 0; if (++row == VGA_HEIGHT) row = 0; }
}

void write(const char* s) {
	for (size_t i = 0; i < strlen(s); i++) put(s[i]);
}

void kernel_main(void) {
	color = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) VGA[i] = (uint16_t) ' ' | (uint16_t) color << 8;
	row = col = 0;
	write("Hello, World!\n");
	write("Welcome to my OS.\n");
	write("I love you!!!\n");
}