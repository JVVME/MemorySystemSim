//
// Created by zhang on 2025/1/7.
//
#pragma once
#include <stdbool.h>

#include "definition.h"
#include "cache.h"
#include<stdint.h>

#ifndef MMU_H
#define MMU_H

#define PAGE_OFFSET_DIGIT (MACHINE_DIGIT - 1 - __builtin_clz(PAGE_SIZE))
#define PM_FRAME_DIGIT PM_DIGIT - PAGE_OFFSET_DIGIT
#define VM_FRAME_DIGIT VM_DIGIT - PAGE_OFFSET_DIGIT
#define PAGE_OFFSET_MASK ((1 << PAGE_OFFSET_DIGIT) - 1)
#define PM_FRAME_MASK (((1 << PM_FRAME_DIGIT) - 1) << PAGE_OFFSET_DIGIT)
#define VM_FRAME_MASK (((1 << VM_FRAME_DIGIT) - 1) << PAGE_OFFSET_DIGIT)
#define PM_EXC_FRAME_MASK ((1 << PM_FRAME_DIGIT) - 1)
#define EXC_VALID_LOC 15
#define PI_N_BYTES 2
#define TLB_HIT_LATENCY (MACHINE_CLOCK)
#define TLB_MISS_DELAY (TLB_SIZE * MACHINE_CLOCK + PHYSICAL_MEMORY_ACCESS_LATENCY)

typedef struct {
    uint32_t vpn;
    uint32_t ppn;
    bool valid;
    bool read;
    bool dirty;
}TLB_Entry;


void TLB_init();
void page_table_init();
uint32_t v_addr_2_p_addr(uint32_t v_addr);
uint32_t choose_to_exchange();
void exchange(uint32_t vm_frame, uint32_t pi_addr, uint32_t cte);
uint8_t mmu_rf_memory(uint32_t p_addr);



#endif //MMU_H
