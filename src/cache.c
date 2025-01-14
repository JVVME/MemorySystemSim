//
// Created by zhang on 2024/12/31.
//

#include "../include/cache.h"
// #include <stdio.h>


cache_line L1_cache[NUM_L1_CACHE_LINES];
uint8_t p_memory[NUM_PHYSICAL_MEMORY_BLOCKS][PHYSICAL_MEMORY_BLOCK_SIZE];


void cache_init() {
    for (int i = 0; i < NUM_L1_CACHE_LINES; i++) {
        L1_cache[i].valid = false;
        L1_cache[i].dirty = false;
        L1_cache[i].lru = i % LRU_NUM;
    }
}

void lru_update(uint32_t begin, uint32_t end, uint32_t index) {
    uint8_t target = L1_cache[index].lru;
    for (uint32_t i = begin; i < end; i++) {
        if (L1_cache[i].lru < target) {
            L1_cache[i].lru++;
        }
    }
    L1_cache[index].lru = 0;
}

void lru_join(uint32_t begin, uint32_t end, uint32_t index) {
    for (uint32_t i = begin; i < end; i++) {
        L1_cache[i].lru++;
    }
    L1_cache[index].lru = 0;
}

void cache_rf_memory(uint32_t block_index, uint32_t cache_index) {
    for (uint32_t i = 0; i < PHYSICAL_MEMORY_BLOCK_SIZE; i++) {
        L1_cache[cache_index].cache_block[i] = p_memory[block_index][i];
    }
}

void cache_rt_memory(uint32_t block_index, uint32_t cache_index) {
    for (uint32_t i = 0; i < PHYSICAL_MEMORY_BLOCK_SIZE; i++) {
         p_memory[block_index][i] = L1_cache[cache_index].cache_block[i];
    }
}

uint32_t choose_to_swap(uint32_t begin, uint32_t end) {
    for (uint32_t i = begin; i < end; i++) {
        if (L1_cache[i].valid == false) {
            return i;
        }
    }

    for (uint32_t i = begin; i < end; i++) {
        if (L1_cache[i].lru == LRU_NUM - 1) {
            return i;
        }
    }

    return -1;
}

void cache_exchange(uint32_t p_address, uint32_t cts) {
    uint32_t begin = ((p_address  & GROUP_MASK) >> L1_OFFSET_DIGIT ) * GROUP_CAP;
    uint32_t end = (((p_address  & GROUP_MASK) >> L1_OFFSET_DIGIT ) + 1) * GROUP_CAP;
    if (L1_cache[cts].valid == false) {
        cache_rf_memory(p_address >> L1_OFFSET_DIGIT, cts);
        L1_cache[cts].tag = (p_address & TAG_MASK) >> (L1_OFFSET_DIGIT + L1_GROUP_DIGIT);
        lru_join(begin, end, cts);
        L1_cache[cts].dirty = false;
        L1_cache[cts].valid = true;
    }
    else {
        if (L1_cache[cts].dirty) {
            cache_rt_memory((L1_cache[cts].tag << L1_GROUP_DIGIT) + (cts % GROUP_NUM), cts);
            L1_cache[cts].dirty = false;
        }
        cache_rf_memory(p_address >> L1_OFFSET_DIGIT, cts);
        L1_cache[cts].tag = (p_address & TAG_MASK) >> (L1_OFFSET_DIGIT + L1_GROUP_DIGIT);
        lru_update(begin, end, cts);
        L1_cache[cts].valid = true;
    }
}

uint8_t cache_read(uint32_t p_address) {
    const uint8_t offset = p_address & OFFSET_MASK;
    const uint8_t group_num = (p_address  & GROUP_MASK) >> L1_OFFSET_DIGIT;
    const uint16_t tag = (p_address & TAG_MASK) >> (L1_OFFSET_DIGIT + L1_GROUP_DIGIT);

    const uint32_t b_group = group_num * GROUP_CAP;
    const uint32_t e_group = (group_num + 1) * GROUP_CAP;

    uint8_t data = 0;
    bool hit = false;

    for (uint32_t i = b_group; i < e_group; i++) {
        if (tag == L1_cache[i].tag && L1_cache[i].valid == true) {
            data = L1_cache[i].cache_block[offset];
            hit = true;
            lru_update(b_group, e_group, i);
            break;
        }
    }

    time_count += L1_CACHE_HIT_LATENCY;

    if (!hit) {
        uint32_t cts = choose_to_swap(b_group, e_group);
        cache_exchange(p_address, cts);
        data = L1_cache[cts].cache_block[offset];
        time_count += L1_CACHE_MISS_DELAY;
    }

    return data;

}

void cache_write(uint32_t p_address, uint8_t data) {
    const uint8_t offset = p_address & OFFSET_MASK;
    const uint8_t group_num = (p_address  & GROUP_MASK) >> L1_OFFSET_DIGIT;
    const uint16_t tag = (p_address & TAG_MASK) >> (L1_OFFSET_DIGIT + L1_GROUP_DIGIT);

    const uint32_t b_group = group_num * GROUP_CAP;
    const uint32_t e_group = (group_num + 1) * GROUP_CAP;

    for (uint32_t i = b_group; i < e_group; i++) {
        if (L1_cache[i].tag == tag && L1_cache[i].valid) {
            L1_cache[i].cache_block[offset] = data;
            L1_cache[i].dirty = true;
            lru_update(b_group, e_group, i);
            time_count += L1_CACHE_HIT_LATENCY;
            return ;
        }
    }

    uint32_t cts = choose_to_swap(b_group, e_group);
    cache_exchange(p_address, cts);

    L1_cache[cts].cache_block[offset] = data;
    L1_cache[cts].dirty = true;

    time_count += L1_CACHE_MISS_DELAY;

}

void cache_flush() {
    for (uint32_t i = 0; i < NUM_L1_CACHE_LINES; i++) {
        if (L1_cache[i].valid && L1_cache[i].dirty) {
            cache_rt_memory((L1_cache[i].tag << L1_GROUP_DIGIT) + (i % GROUP_NUM), i);
            L1_cache[i].valid = false;
        }
    }

}

