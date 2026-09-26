#include "pmm.h"
#include "kprintf.h"

uint32_t *pmm_bitmap = 0;
uint32_t total_frames = 0;
uint32_t used_frames = 0;

void pmm_init(multiboot_info_t *mboot_info){
    if (!mboot_info) {
        kprintf(" Error: no multiboot_info pointer\n");
        return;
    }

    uint64_t max_addr = 0;
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mboot_info->mmap_addr;
    uint32_t mmap_end = mboot_info->mmap_addr + mboot_info->mmap_length;
    while ((uint32_t)mmap < mmap_end){
        if (mmap->type == 1){
            uint64_t base   = ((uint64_t)mmap->base_addr_high << 32) | mmap->base_addr_low;
            uint64_t length = ((uint64_t)mmap->length_high    << 32) | mmap->length_low;
            uint64_t entry_end = base + length;

            if (entry_end > max_addr) {
                max_addr = entry_end;
            }
        }
        mmap = (multiboot_memory_map_t *) ((uint32_t)mmap + mmap->size + sizeof(mmap->size));
    }

    total_frames = max_addr/FRAME_SIZE;
    used_frames = total_frames;
    
    pmm_bitmap = (uint32_t *)&kernel_end;
    uint32_t bitmap_size = (total_frames+31)/32;
    for (uint32_t i = 0; i < bitmap_size; i++){ 
        pmm_bitmap[i] = 0xFFFFFFFF;
    }

    mmap = (multiboot_memory_map_t *) mboot_info->mmap_addr;
    while ((uint32_t)mmap < mmap_end){
        if (mmap->type == 1){
            uint64_t start_addr   = ((uint64_t)mmap->base_addr_high << 32) | mmap->base_addr_low;
            uint64_t length = ((uint64_t)mmap->length_high    << 32) | mmap->length_low;
            uint32_t start_frame = start_addr/FRAME_SIZE;
            uint32_t end_frame = (start_addr+length)/FRAME_SIZE;

            for (uint32_t frame = start_frame; frame < end_frame; frame++){
                if (pmm_test_frame(frame) == 1){
                    pmm_clear_frame(frame);
                    used_frames--;
                }
            }
        }
        mmap = (multiboot_memory_map_t *) ((uint32_t)mmap + mmap->size + sizeof(mmap->size));
    }

    uint32_t bitmap_bytes = (total_frames+7)/8;
    uint32_t bitmap_end_addr = ((uint32_t)&kernel_end) + bitmap_bytes;
    uint32_t bitmap_end_frame = (bitmap_end_addr+FRAME_SIZE) / FRAME_SIZE;
    for (uint32_t frame = 0; frame < bitmap_end_frame; frame++){
        if (pmm_test_frame(frame)==0){
            pmm_set_frame(frame);
            used_frames++;
        }
    }
}

void pmm_set_frame(uint32_t frame){
    uint32_t array_index = GET_BITMAP_INDEX(frame);
    uint32_t bit_offset = GET_BITMAP_OFFSET(frame);

    pmm_bitmap[array_index] |= (1U << bit_offset);
}

void pmm_clear_frame(uint32_t frame){
    uint32_t array_index = GET_BITMAP_INDEX(frame);
    uint32_t bit_offset = GET_BITMAP_OFFSET(frame);

    pmm_bitmap[array_index] &= ~(1U << bit_offset);
}

uint32_t pmm_test_frame(uint32_t frame){
    uint32_t array_index = GET_BITMAP_INDEX(frame);
    uint32_t bit_offset = GET_BITMAP_OFFSET(frame);
    uint32_t is_used = (pmm_bitmap[array_index] & (1U << bit_offset)) != 0;
    return is_used;
}

uint32_t pmm_first_free_frame(){
    uint32_t total_bitmap_elements = total_frames/32;
    for (uint32_t i = 0; i < total_bitmap_elements; i++){
        if (pmm_bitmap[i] == 0xFFFFFFFF){
            continue;
        }
        for (uint32_t j = 0; j < 32; j++){
            uint32_t frame = (i*32)+j;
            if (pmm_test_frame(frame) == 0){
                return frame;
            }
        }
    }
    return 0xFFFFFFFF;
}

uint32_t pmm_alloc_frame(){
    uint32_t frame = pmm_first_free_frame();
    if (frame == 0xFFFFFFFF){
        return 0;
    }

    pmm_set_frame(frame);
    used_frames++;
    return frame * FRAME_SIZE;
}

void pmm_free_frame(uint32_t addr){
    uint32_t frame = addr/FRAME_SIZE;
    if (pmm_test_frame(frame) == 1){
        pmm_clear_frame(frame);
        used_frames--;
    }
}

uint32_t pmm_get_total_frames(){
    return total_frames;
}
uint32_t pmm_get_used_frames(){
    return used_frames;
}