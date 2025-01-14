#include <stdio.h>
#include "include/definition.h"
#include "include/cache.h"
#include "include/register.h"
#include<stdlib.h>
#include<time.h>
#include<assert.h>

#include "include/mmu.h"
#include "include/cache.h"
#include "include/mmu.h"

double time_count = 0;

void cache_init_test();
void cache_read_test();
void cache_write_test();
void tlb_test();
void pre_test();
void program_run(int test_count, int write_scale);
void info_print();

int main(void) {
    pre_test();

    cache_init();
    cr3_init();
    TLB_init();
    page_table_init();

    time_count = 0.0;
    int test_count = 1024 * 4;
    int write_scale = 1024 * 16;

    info_print();
    printf("Data write scale: %dB %dKB\n", write_scale, write_scale / 1024);
    printf("Total test of reading cache: %d \n", test_count);
    printf("------------------------------------------------------------------\n");

    program_run(test_count, write_scale);

    printf("Time:\n%lf ns\n%lf ms\n%lf s\n\t ", time_count, time_count / 1000, time_count / 1000000);

    return 0;
}

void cache_write_test() {
    srand(time(NULL));
    int x = 1024 * 6;
    uint8_t tr[x];
    for (int i = 0; i < x; i++) {
        mem_set(i, i);
        tr[i] = i;
    }
    for (int i = 0; i < x; i+=rand()%10) {
        uint8_t d = rand()%10;
        cache_write(i, d);
        // printf("change address %u to %u\n", i, d);
        tr[i] = d;
    }
    // show_cache();
    // cache_flush();
    // for (int i = 0; i < x; i++) {
    //     printf("cache: %u  ", cache_read((i)));
    //     printf("memory: %u \n", mem_read((i)));
    // }

    for (int i = 0; i < x; i++) {

        // printf("%u %u %u\n", i, cache_read(i), tr[i]);
        // show_cache();
        assert(cache_read(i) == tr[i]);
    }
    // show_cache();
    printf("Cache write test: program run correct. \n");


}

void cache_read_test() {
    int x = 1024 * 6;
    uint8_t tr[x];
    srand(time(NULL));
    for (int i = 0; i< x; i++) {

        uint8_t x = rand() & 0xFF;
        mem_set(i, x);
        tr[i] = x;
        // printf("set memory at %d, data: %u \n", i, x);
    }

    for (int i = 0; i < x; i++) {
        uint32_t addr = i;
        uint8_t data = cache_read(addr);
        // printf("addr: %u, data: %u \n", addr, data);
        assert(data == tr[addr]);
    }

    printf("Cache read test: program run correct. \n");

}

void tlb_test() {
    // for (int i = 0; i < 10; i+= 2) {
    //     printf("%u %u \n", mem_read(i), mem_read((i + 1)));
    // }
    int x = 4096 * 4;
    int tr[x];
    // printf("%u \n", v_addr_2_p_addr(0));
    for (int i = 0; i < x; i++) {
        mem_set(8192 + i, i % 100);
        tr[i] = i % 100;
    }

    for (uint32_t i = 0; i < x; i++) {
        // printf("%u %u\n", 8192 + i, i);
        uint32_t p_addr = v_addr_2_p_addr(i);
        int res = cache_read(p_addr);
        assert(res == tr[i]);
    }
    printf("TLB test: program run correct. \n");
}

void cache_init_test() {
    srand(time(NULL));  // 初始化随机数生成器
    // 生成并打印 10 个物理地址
    for (int i = 0; i < 10; i++) {
        // 生成一个随机的 30 位物理地址（0 到 0x3FFFFFFF）
        uint32_t p_address = rand() & 0x3FFFFFFF;

        // 打印原始物理地址
        // printf("Physical Address 0x%X:\n", p_address);

        cache_read(p_address);
    }
    printf(" Cache init test: program run correct. \n");
}

void pre_test() {
    cache_init();
    cr3_init();
    TLB_init();
    page_table_init();

    tlb_test();
    cache_read_test();
    cache_write_test();
    printf("------------------------------------------------------------------\n");
}

void program_run(int test_count, int write_scale) {
    int offset = 8192;
    srand(time(NULL));
    for (int i = 0; i< write_scale; i++) {
        uint8_t d = rand() & 0xFF;
        mem_set(i + offset, d);

    }
    for (int i = 0; i < test_count; i++) {
        uint32_t addr = rand() % write_scale;
        cache_read(v_addr_2_p_addr(addr));
    }
}

void info_print() {
    printf("TEST SETTING INFO  (UNIT: BYTES)\n");
    printf("memory page size: %d\nvirtual memory page: %d\tvirtual memory size: %d\n", PAGE_SIZE, NUM_PAGES, VIRTUAL_MEMORY_SIZE);
    printf("physical memory blocks: %d\t physical memory size: %d\n", PHYSICAL_MEMORY_BLOCK_SIZE, PHYSICAL_MEMORY_SIZE);
    printf("cache size: %d\t N-way-set: %d\n", L1_CACHE_SIZE, GROUP_CAP);
    printf("TLB size: %d  using clock algorithm\n", TLB_SIZE);
    printf("------------------------------------------------------------------\n");

}
