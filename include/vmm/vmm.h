#pragma once

#include "vmm/kheap.h"
#include "state.h"


void init_vmx_all_cpus(kheap_metadata_t *kheap, cpu_state_t *state);
void init_vmx_on_cpu(kheap_metadata_t *kheap, cpu_state_t *state);
void init_vmx_current_cpu(kheap_metadata_t *kheap, cpu_state_t *state);
void enter_vmx(void);

void configure_vmcs(cpu_state_t *state);
void vmexit_handler(void);
void vmenter_handler(void);
