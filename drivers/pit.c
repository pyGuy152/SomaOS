#include "pit.h"
#include "io.h"

static volatile uint32_t timer_ticks = 0;
static uint32_t target_frequency = 100;

void pit_handler(){
    timer_ticks++;
    outb(0x20, 0x20);
}

void pit_init(uint32_t frequency){
    target_frequency = frequency;

    uint32_t divisor = 1193182/frequency;

    outb(0x43, 0x36);

    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

uint32_t pit_get_total_ticks(void){
    return timer_ticks;
}

void sleep(uint32_t milliseconds){
    uint32_t start_ticks = timer_ticks;
    uint32_t ticks_to_wait = (milliseconds * target_frequency)/1000;

    while (timer_ticks < (start_ticks+ticks_to_wait)){
        asm volatile("sti; hlt");
    }
}