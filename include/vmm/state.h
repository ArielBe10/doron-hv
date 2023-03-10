#pragma once 

#include <stddef.h>

#include "vmm/kheap.h"
#include "vmm/paging.h"

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
    paging_tables_t paging_tables;
    ept_paging_tables_t ept_paging_tables;
    size_t gdt[0xff];

    int cpu_count;
    cpu_state_t *cpu_states[];
};


cpu_shared_data_t *create_cpu_data(kheap_metadata_t *kheap);
void set_cpu_data(cpu_shared_data_t *shared_data);