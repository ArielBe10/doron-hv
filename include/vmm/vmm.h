#pragma once

#include "vmm/kheap.h"
#include "state.h"


void enter_vmx(kheap_metadata_t *kheap, cpu_state_t *state);
void configure_vmcs(cpu_state_t *state);
void vmexit_handler(void);
void vmenter_handler(void);
