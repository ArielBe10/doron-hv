#pragma once 

#include <stddef.h>

#include "vmm/kheap.h"


#define PAGE_SIZE 0x1000
#define PAGE_TABLE_LENGTH 512
#define STACK_SIZE 0x10000


typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rsp;
    uint64_t rip;
} registers_t;


typedef struct _cpu_data_t cpu_data_t;
typedef struct _cpu_shared_data_t cpu_shared_data_t;


typedef struct {
    registers_t registers;
    char stack[STACK_SIZE];
    cpu_data_t *cpu_data;
} cpu_state_t;


struct _cpu_data_t{
    char vmxon[0x1000];
    char vmcs[0x1000];
    int cpu_id;
    cpu_shared_data_t *shared_data;
};


struct _cpu_shared_data_t {
    size_t pml4_tables[PAGE_TABLE_LENGTH];  // 1 pml4 table
    size_t pdp_tables[PAGE_TABLE_LENGTH];  // 512 pdp tables
    size_t pd_tables[PAGE_TABLE_LENGTH * PAGE_TABLE_LENGTH];  // 512 pd for every pdp
    size_t gdt[0xff];

    int cpu_count;
    cpu_state_t *cpu_states[];
};


cpu_shared_data_t *create_cpu_data(kheap_metadata_t *kheap);
void set_cpu_data(cpu_shared_data_t *shared_data);