#pragma once 

#include <stddef.h>

#include "vmm/kheap.h"


#define PAGE_SIZE 0x1000
#define PAGE_TABLE_LENGTH 512
#define STACK_SIZE 0x10000


typedef struct _shared_cpu_state_t shared_cpu_state_t;


typedef struct {
    char vmxon[0x1000];
    char vmcs[0x1000];

    char stack[STACK_SIZE];

    size_t pml4_tables[PAGE_TABLE_LENGTH];  // 1 pml4 table
    size_t pdp_tables[PAGE_TABLE_LENGTH];  // 512 pdp tables
    size_t pd_tables[PAGE_TABLE_LENGTH * PAGE_TABLE_LENGTH];  // 512 pd for every pdp
    size_t gdt[0xff];

    shared_cpu_state_t *shared_cpu_state;
    int cpu_id;

} single_cpu_state_t;


struct _shared_cpu_state_t {
    int cpu_count;
    single_cpu_state_t *single_cpu_states[];
};


shared_cpu_state_t *create_cpu_states(kheap_metadata_t *kheap);
void configure_cpu_states(shared_cpu_state_t *shared_states);