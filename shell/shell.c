#include "shell.h"
#include "string.h"
#include "vga.h"

static char input_buffer[256];
static uint8_t buffer_index = 0;

void shell_execute(){
    vga_put('\n');
    vga_write(input_buffer);
    vga_put('\n');
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