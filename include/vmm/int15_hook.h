#pragma once

#include "vmm/state.h"
#include "vmm/bios.h"


#define MAX_MMAP_ENTRIES 16


void setup_int15_hook(cpu_shared_data_t *shared_data);
void fake_int15_handler(cpu_state_t *state);
int mark_mmap_memory_unuseable(e820_mmap_t *mmap, size_t start, size_t length);