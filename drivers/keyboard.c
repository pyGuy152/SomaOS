#include "keyboard.h"
#include "../shell/shell.h"
#include "vga.h"
#include "io.h"

static const char scancode_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
  '*',   0, ' '
};

static const char scancode_ascii_uppercase[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
  '*',   0, ' '
};

static bool shift_pressed = false;
static bool caps_lock_active = false;

void keyboard_handler(void){
    uint8_t scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36){
        shift_pressed = true;
        goto eoi;
    }
    if (scancode == 0xAA || scancode == 0xB6){
        shift_pressed = false;
        goto eoi;
    }
    
    if (scancode == 0x3A || scancode == 0xBA){
        caps_lock_active = !caps_lock_active;
        goto eoi;
    }

    if (scancode & 0x80){
        goto eoi;
    }

    if (scancode < sizeof(scancode_ascii)){
        char c = scancode_ascii[scancode];
        bool is_letter = (c >= 'a' && c <= 'z');
        if (shift_pressed || (is_letter && caps_lock_active)){
            c = scancode_ascii_uppercase[scancode];
        }
        if (c != 0){
            shell_handle_char(c);
        }
    }
    

eoi:
    outb(0x20, 0x20);
}