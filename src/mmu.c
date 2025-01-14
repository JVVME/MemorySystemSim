//
// Created by zhang on 2024/12/31.
//

#include "../include/mmu.h"

#include <assert.h>
#include <stdio.h>

#include "../include/register.h"

TLB_Entry TLB[TLB_SIZE];



void page_table_init() {
    // change if PI_N_BYTES change!!
    /*
     *页表从0x0地址开始 为其分配128页面 地址8192(页框2)开始  页框从0开始
     */
    for (uint32_t i = 0; i < 128; i++) {
        uint32_t data = 1;
        data <<= EXC_VALID_LOC;
        data = data + 2 + i;
        uint8_t data1 = (data >> 8) & 0xFF;
        uint8_t data2 = data & 0xFF;
        // printf("init :%u   %u %u \n", data, data1, data2);
        mem_set(i * 2, data1);
        mem_set(i * 2 + 1, data2);
    }


}

void TLB_init() {
    for (int i = 0; i < TLB_SIZE; i++) {
        TLB[i].valid = false;
    }

}

uint32_t choose_to_exchange() {
    bool done = false;
    uint32_t res = 0;
    for (uint32_t i = 0; i < TLB_SIZE; i++) {
        if (TLB[i].valid == false) {
            res = i;
            done = true;
            break;
        }
    }
    if (!done) {
            for (uint32_t i = 0; i < TLB_SIZE; i++) {
                if (TLB[i].dirty == false) {
                    res = i;
                    done = true;
                    break;
                }
            }
    }
    return res;
}

void exchange(uint32_t vm_frame, uint32_t pi_addr, uint32_t cte) {
    uint8_t data1 = mmu_rf_memory(pi_addr);
    uint8_t data2 = mmu_rf_memory(pi_addr + 1);
    uint32_t data = data1;
    data <<= 8;
    data += data2;

    uint32_t pm_frame = data & PM_EXC_FRAME_MASK;
    bool valid = data >> EXC_VALID_LOC;
    if (!valid) {
        printf("SHOULDN'T BE HERE!\n");
        assert(0);
    }

    TLB[cte].ppn = pm_frame;
    TLB[cte].vpn = vm_frame;
    TLB[cte].dirty = false;
    TLB[cte].read = true;
    TLB[cte].valid = true;

}

uint8_t mmu_rf_memory(uint32_t p_addr) {
    uint32_t block = p_addr >> L1_OFFSET_DIGIT;
    uint32_t offset = p_addr & OFFSET_MASK;
    return p_memory[block][offset];
}

uint32_t v_addr_2_p_addr(uint32_t v_addr) {
    uint32_t vm_frame = (v_addr & VM_FRAME_MASK) >> PAGE_OFFSET_DIGIT;
    uint32_t page_offset = v_addr & PAGE_OFFSET_MASK;

    uint32_t p_addr = 0;
    bool hit = false;

    for (int i = 0; i < TLB_SIZE; i++) {
        if (TLB[i].vpn == vm_frame && TLB[i].valid) {
            p_addr = TLB[i].ppn;
            TLB[i].read = true;
            hit = true;
            break;
        }
    }

    time_count += TLB_HIT_LATENCY;

    if (!hit) {
        uint32_t cte = choose_to_exchange();
        uint32_t base_addr = cr3_read();
        uint32_t pi_addr = base_addr + vm_frame * PI_N_BYTES;
        exchange(vm_frame, pi_addr, cte);
        p_addr = TLB[cte].ppn;
        hit = true;
        time_count += TLB_MISS_DELAY;
    }

    p_addr <<= PAGE_OFFSET_DIGIT;
    p_addr += page_offset;

    return p_addr;
}

