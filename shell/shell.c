#include "shell.h"
#include "string.h"
#include "kprintf.h"
#include "vga.h"
#include "io.h"
#include "pit.h"

static char input_buffer[256];
static uint8_t buffer_index = 0;

void shell_execute(){
    if (streq(input_buffer,"help")){
        vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
        vga_write(" info - information on this OS\n uptime - total running time\n clear - clears the screen\n reboot - reboot Soma Os\n");
    }else if (streq(input_buffer,"info")){
        vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
        vga_write(" Welcome to Soma OS. This is a OS build by Sarvesh and Alex.\n");
    }else if (streq(input_buffer,"clear")){
        vga_clear();
    }else if (streq(input_buffer,"uptime")){
        int ticks = pit_get_total_ticks();
        int total_seconds = ticks/100;

        int seconds = total_seconds%60;
        int minutes = total_seconds%(60*60)/60;
        int hours = total_seconds/(60*60);

        vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
        vga_write(" System Uptime:\n");
        kprintf(" %dh:%dm:%ds\n", hours, minutes, seconds);

    }else if (streq(input_buffer,"reboot")){
        vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
        vga_write(" Rebooting SomaOS...\n");
        memset(input_buffer, 0, sizeof(input_buffer));
        buffer_index = 0;
        outb(0x64, 0xFE);
        
    }else{
        vga_set_color(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_WHITE));
        vga_write(" Error - Command Not Found. Try help for list of commands\n");
    }
    vga_set_color(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
    memset(input_buffer, 0, sizeof(input_buffer));
    buffer_index = 0;
}

void shell_handle_char(char c){
    vga_put(c);
    if (c == '\n'){
        shell_execute();
    }else if (c == '\b' && buffer_index > 0){
        input_buffer[--buffer_index] = '\0';
    }else{
        input_buffer[buffer_index] = c;
        buffer_index++;
    }
}