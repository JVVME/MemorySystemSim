//
// Created by zhang on 2024/12/31.
//

#include "../include/main_memory.h"

#include <stdio.h>


void mem_set(uint32_t addr, uint8_t data) {
    uint32_t blocks = addr >> 6;
    uint8_t offset = addr & 0x3F;

    p_memory[blocks][offset] = data;
}

uint8_t mem_read(uint32_t addr) {
    uint32_t blocks = addr >> 6;
    uint8_t offset = addr & 0x3F;
    return p_memory[blocks][offset];
}
