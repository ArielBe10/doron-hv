#pragma once

#include "vmm/state.h"

#define VMCALL_INT15_GATE 0x2000


int handle_vm_hlt(cpu_state_t *state);
int handle_ept_violation(cpu_state_t *state);
int handle_vmcall(cpu_state_t *state);
void init_hv(cpu_state_t *state);
