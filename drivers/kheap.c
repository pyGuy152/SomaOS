#include "kheap.h"
#include "pmm.h"

static heap_block_t* heap_start;

void kheap_init(){
    uint32_t addr = pmm_alloc_frame();
    heap_start = (heap_block_t*) addr;

    heap_start->size = FRAME_SIZE - sizeof(heap_block_t);
    heap_start->is_free = 1;
    heap_start->prev = NULL;
    heap_start->next = NULL;
}

void* kmalloc(uint32_t size){
    if (size == 0){
        return NULL;
    }
    heap_block_t* useable_block = heap_start;

    while (!(useable_block->is_free == 1 && useable_block->size >= size)){
        if (!useable_block->next){
            return NULL;
        }
        useable_block = useable_block->next;
    }

    // split block if more than size
    if (useable_block->size >= size+sizeof(heap_block_t)+4){
        heap_block_t* new_block = (heap_block_t*)((uint8_t*)(useable_block + 1) + size);
        
        new_block->size = useable_block->size-size-sizeof(heap_block_t);
        new_block->is_free = 1;
        new_block->next = useable_block->next;

        new_block->prev = useable_block;
        if (new_block->next){
            new_block->next->prev = new_block;
        }
        
        useable_block->size = size;
        useable_block->next = new_block;
    }

    useable_block->is_free = 0;
    return (void*)(useable_block + 1);
}

void kfree(void* ptr){
    if (!ptr){
        return;
    }
    heap_block_t* heap_block = (heap_block_t*)((uint8_t*)ptr - sizeof(heap_block_t));
    heap_block->is_free = 1;

    // merge blocks
    if (heap_block->next && heap_block->next->is_free == 1){
        heap_block->size += (heap_block->next->size + sizeof(heap_block_t));
        heap_block->next = heap_block->next->next;
        heap_block->next->prev = heap_block;
    }
    if (heap_block->prev && heap_block->prev->is_free == 1){
        heap_block = heap_block->prev;
        heap_block->size += (heap_block->next->size + sizeof(heap_block_t));
        heap_block->next = heap_block->next->next;
        heap_block->next->prev = heap_block;
    }
}

uint32_t get_heap_block_count(){
    uint32_t heap_block_count = 1;

    heap_block_t* current = heap_start;
    while (current->next){
        heap_block_count++;
        current = current->next;
    }
    return heap_block_count;
}