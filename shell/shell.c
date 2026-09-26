#include "shell.h"
#include "string.h"
#include "kprintf.h"
#include "vga.h"
#include "io.h"
#include "pit.h"
#include "pmm.h"

static char input_buffer[256];
static uint8_t buffer_index = 0;

// command functions
void cmd_help(){
    vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
    vga_write(" info - information on this OS\n test memory - tests memory allocation\n uptime - total running time\n clear - clears the screen\n reboot - reboot Soma Os\n");
}

void cmd_info(){
    vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
    vga_write(" Welcome to Soma OS. This is a OS build by Sarvesh and Alex.\n");
}

void cmd_clear(){
    vga_clear();
}

void cmd_test_memory(){
    vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
    uint32_t addr = pmm_alloc_frame();
    uint32_t used_frames = pmm_get_used_frames();
    kprintf(" Reserved a frame Total memory: %dMB Used memory: %dKB\n",pmm_get_total_frames()/256,used_frames*4);
    pmm_free_frame(addr);
    uint32_t new_used_frames = pmm_get_used_frames();
    kprintf(" Freed a frame Total memory: %dMB Used memory: %dKB\n",pmm_get_total_frames()/256,pmm_get_used_frames()*4);
    if (used_frames-new_used_frames == 1){
        kprintf("\n Memory allocation works!\n");
    }
}

void cmd_uptime(){
    uint32_t ticks = pit_get_total_ticks();
    uint32_t total_seconds = ticks/100;

    uint32_t seconds = total_seconds%60;
    uint32_t minutes = total_seconds%(60*60)/60;
    uint32_t hours = total_seconds/(60*60);

    vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
    vga_write(" System Uptime:\n");
    kprintf(" %dh:%dm:%ds\n", hours, minutes, seconds);
}

void cmd_reboot(){
    vga_set_color(vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_WHITE));
    vga_write(" Rebooting SomaOS...\n");
    memset(input_buffer, 0, sizeof(input_buffer));
    buffer_index = 0;
    outb(0x64, 0xFE);
}

// command handler
void shell_execute(){
    if (streq(input_buffer,"help")){
        cmd_help();
    }else if (streq(input_buffer,"info")){
        cmd_info();
    }else if (streq(input_buffer,"clear")){
        cmd_clear();
    }else if (streq(input_buffer,"test memory")){
        cmd_test_memory();
    }else if (streq(input_buffer,"uptime")){
        cmd_uptime();
    }else if (streq(input_buffer,"reboot")){
        cmd_reboot();
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