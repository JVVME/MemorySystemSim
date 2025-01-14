//
// Created by zhang on 2024/12/31.
//

#pragma once
#include<stdbool.h>
#include<stdint.h>
#include"definition.h"
#include "main_memory.h"

#ifndef CACHE_H
#define CACHE_H


// L1 采用 GROUP_CAP 路组相联
#define GROUP_CAP 2
#define LRU_NUM GROUP_CAP
#define GROUP_NUM CACHE_LINE_SIZE / GROUP_CAP
#define L1_OFFSET_DIGIT (MACHINE_DIGIT - 1 - __builtin_clz(CACHE_LINE_SIZE))
#define L1_GROUP_DIGIT (MACHINE_DIGIT - 1 - __builtin_clz(GROUP_NUM))
#define OFFSET_MASK ((1 << L1_OFFSET_DIGIT) -1)
#define GROUP_MASK ((1 << (L1_GROUP_DIGIT + L1_OFFSET_DIGIT)) - 1 - OFFSET_MASK)
#define TAG_MASK (((1 << (PM_DIGIT - L1_GROUP_DIGIT - L1_OFFSET_DIGIT)) - 1) << (L1_GROUP_DIGIT + L1_OFFSET_DIGIT))

#define L1_CACHE_HIT_LATENCY (L1_CACHE_ACCESS_LATENCY + GROUP_CAP * MACHINE_CLOCK)
#define L1_CACHE_MISS_DELAY (LRU_NUM * MACHINE_CLOCK + PHYSICAL_MEMORY_ACCESS_LATENCY)

typedef struct {
    bool valid;
    bool dirty;
    uint8_t lru;
    uint32_t tag;
    uint8_t cache_block[CACHE_LINE_SIZE];
} cache_line;


void cache_init();
uint8_t cache_read(uint32_t p_address);
void cache_write(uint32_t p_address, uint8_t data);
void cache_flush();
void cache_exchange(uint32_t p_address, uint32_t cts);
void lru_update(uint32_t begin, uint32_t end, uint32_t index);
void lru_join(uint32_t begin, uint32_t end, uint32_t index);
void cache_rf_memory(uint32_t block_index, uint32_t cache_index);     //rf = read from
void cache_rt_memory(uint32_t block_index, uint32_t cache_index);     //rt = write to
uint32_t choose_to_swap(uint32_t begin, uint32_t end);



#endif //CACHE_H
