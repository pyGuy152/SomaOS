#ifndef PMM_H
#define PMM_H
#include <stdint.h>

#define FRAME_SIZE 4096
#define GET_BITMAP_INDEX(frame) ((frame) / 32)
#define GET_BITMAP_OFFSET(frame) ((frame) % 32)

extern uint32_t kernel_end;

// EBX
typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
    uint32_t mmap_length; // size of memory map 
    uint32_t mmap_addr; // pointer to memory map array
} __attribute__((packed)) multiboot_info_t;

typedef struct {
    uint32_t size;
    uint32_t base_addr_low;
    uint32_t base_addr_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type; // 1 = free
} __attribute__((packed)) multiboot_memory_map_t;

void pmm_init(multiboot_info_t *mboot_info);
void pmm_set_frame(uint32_t frame);
void pmm_clear_frame(uint32_t frame);
uint32_t pmm_test_frame(uint32_t frame);
uint32_t pmm_first_free_frame();

uint32_t pmm_alloc_frame();
void pmm_free_frame(uint32_t addr);

uint32_t pmm_get_total_frames();
uint32_t pmm_get_used_frames();

#endif