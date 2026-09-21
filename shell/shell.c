#include "shell.h"
#include "string.h"
#include "vga.h"

static char input_buffer[256];
static uint8_t buffer_index = 0;

void shell_execute(){
    vga_put('\n');
    if (streq(input_buffer,"help")){
        vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
        vga_write(" clear - clears the screen\n");
    }else if (streq(input_buffer,"clear")){
        vga_clear();
    }else{
        vga_set_color(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_WHITE));
        vga_write(" Error - Command Not Found. Try help for list of commands\n");
    }
    vga_set_color(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
    memset(input_buffer, 0, sizeof(input_buffer));
    buffer_index = 0;
}

void shell_handle_char(char c){
    if (c == '\n'){
        shell_execute();
    }else{
        input_buffer[buffer_index] = c;
        buffer_index++;
        vga_put(c);
    }
}