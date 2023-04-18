#pragma once

#include "vmm/state.h"

#include <stdbool.h>


int update_msr_access_policy(cpu_shared_data_t *shared_data, uint64_t msr_number, bool readable, bool writable);