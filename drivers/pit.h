#ifndef PIT_H
#define PIT_H

#include <stdint.h>

void pit_init(uint32_t frequency);

uint32_t pit_get_total_ticks(void);
void sleep(uint32_t milliseconds);

#endif