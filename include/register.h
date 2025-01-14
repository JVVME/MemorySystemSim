//
// Created by zhang on 2025/1/7.
//

#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include"definition.h"


extern uint32_t CR3;

void cr3_init();
void cr3_set(uint32_t p_addr);
uint32_t cr3_read();

#endif //REGISTER_H
