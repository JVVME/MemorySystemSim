//
// Created by zhang on 2024/12/31.
//
#pragma once
#include"definition.h"
#include<stdint.h>

#ifndef MAIN_MEMORY_H
#define MAIN_MEMORY_H


extern uint8_t v_memory[NUM_PAGES][PAGE_SIZE];
extern uint8_t p_memory[NUM_PHYSICAL_MEMORY_BLOCKS][PHYSICAL_MEMORY_BLOCK_SIZE];


void mem_set(uint32_t addr, uint8_t data);
uint8_t mem_read(uint32_t addr);

#endif //MAIN_MEMORY_H
