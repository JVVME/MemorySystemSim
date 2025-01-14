//
// Created by zhang on 2025/1/6.
//

#ifndef DEFINITION_H
#define DEFINITION_H

#define MACHINE_DIGIT 32

// ======================== 内存设置 ========================

// 虚拟内存相关设置
#define PAGE_SIZE 4096                 // 页面大小 4KB
#define NUM_PAGES (1024)       // 系统中有 1024  (1K) 个页面
#define VIRTUAL_MEMORY_SIZE (NUM_PAGES * PAGE_SIZE)  // 系统虚拟内存
#define VM_DIGIT (MACHINE_DIGIT - 1 - __builtin_clz(VIRTUAL_MEMORY_SIZE))

// 物理内存相关设置
#define PHYSICAL_MEMORY_BLOCK_SIZE 64   // 主存块大小 64 字节，与缓存行大小一致
#define PHYSICAL_MEMORY_SIZE (1024 * 1024 * 2)  // 2MB物理内存
#define PM_DIGIT (MACHINE_DIGIT - 1 - __builtin_clz(PHYSICAL_MEMORY_SIZE))
#define NUM_PHYSICAL_MEMORY_BLOCKS (PHYSICAL_MEMORY_SIZE / PHYSICAL_MEMORY_BLOCK_SIZE)  // 物理内存块数


// ======================== 缓存设置 ========================

// 缓存设置
#define CACHE_LINE_SIZE 64                // 缓存行大小 64 字节
#define L1_CACHE_SIZE 4096               // L1 缓存大小 4KB

// L1 缓存
#define NUM_L1_CACHE_LINES (L1_CACHE_SIZE / CACHE_LINE_SIZE)  // L1 缓存行数

// ======================== 处理器设置 ========================

// 处理器相关设置
#define CR3_INIT_ADDR 0x0

// ======================== 页表与虚拟内存设置 ========================

#define TLB_SIZE 4
// 页表设置
#define PAGE_TABLE_ENTRIES (VIRTUAL_MEMORY_SIZE / PAGE_SIZE)  // 页表项数目

// ======================== 系统延迟设置 ========================

#define MACHINE_CLOCK 0.3
#define PHYSICAL_MEMORY_ACCESS_LATENCY 60  // 物理内存访问延迟 10ns
#define L1_CACHE_ACCESS_LATENCY 3      // Cache访问延迟 3ns

extern double time_count;

#endif

