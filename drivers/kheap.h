#ifndef KHEAP_H
#define KHEAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct heap_block{
    uint32_t size;
    uint8_t is_free;
    struct heap_block* next;
    struct heap_block* prev;
} heap_block_t;

void kheap_init();
void* kmalloc(uint32_t size);
void kfree(void* ptr);

uint32_t get_heap_block_count();

#endif