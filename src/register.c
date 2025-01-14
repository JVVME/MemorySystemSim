//
// Created by zhang on 2025/1/7.
//

#include "../include/register.h"

uint32_t CR3;

void cr3_init() {
    cr3_set(CR3_INIT_ADDR);
}

void cr3_set(uint32_t p_addr) {
    CR3 = p_addr;
}

uint32_t cr3_read() {
    return CR3;
}
