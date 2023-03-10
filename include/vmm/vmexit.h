#pragma once

#include "vmm/state.h"


int handle_vm_hlt(cpu_state_t *state);
int handle_ept_violation(cpu_state_t *state);
