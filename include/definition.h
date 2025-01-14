//
// Created by zhang on 2025/1/6.
//

#ifndef DEFINITION_H
#define DEFINITION_H

#define MACHINE_DIGIT 32

// ======================== �ڴ����� ========================

// �����ڴ��������
#define PAGE_SIZE 4096                 // ҳ���С 4KB
#define NUM_PAGES (1024)       // ϵͳ���� 1024  (1K) ��ҳ��
#define VIRTUAL_MEMORY_SIZE (NUM_PAGES * PAGE_SIZE)  // ϵͳ�����ڴ�
#define VM_DIGIT (MACHINE_DIGIT - 1 - __builtin_clz(VIRTUAL_MEMORY_SIZE))

// �����ڴ��������
#define PHYSICAL_MEMORY_BLOCK_SIZE 64   // ������С 64 �ֽڣ��뻺���д�Сһ��
#define PHYSICAL_MEMORY_SIZE (1024 * 1024 * 2)  // 2MB�����ڴ�
#define PM_DIGIT (MACHINE_DIGIT - 1 - __builtin_clz(PHYSICAL_MEMORY_SIZE))
#define NUM_PHYSICAL_MEMORY_BLOCKS (PHYSICAL_MEMORY_SIZE / PHYSICAL_MEMORY_BLOCK_SIZE)  // �����ڴ����


// ======================== �������� ========================

// ��������
#define CACHE_LINE_SIZE 64                // �����д�С 64 �ֽ�
#define L1_CACHE_SIZE 4096               // L1 �����С 4KB

// L1 ����
#define NUM_L1_CACHE_LINES (L1_CACHE_SIZE / CACHE_LINE_SIZE)  // L1 ��������

// ======================== ���������� ========================

// �������������
#define CR3_INIT_ADDR 0x0

// ======================== ҳ���������ڴ����� ========================

#define TLB_SIZE 4
// ҳ������
#define PAGE_TABLE_ENTRIES (VIRTUAL_MEMORY_SIZE / PAGE_SIZE)  // ҳ������Ŀ

// ======================== ϵͳ�ӳ����� ========================

#define MACHINE_CLOCK 0.3
#define PHYSICAL_MEMORY_ACCESS_LATENCY 60  // �����ڴ�����ӳ� 10ns
#define L1_CACHE_ACCESS_LATENCY 3      // Cache�����ӳ� 3ns

extern double time_count;

#endif

