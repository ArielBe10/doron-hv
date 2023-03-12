#pragma once


#include <stddef.h>


#define APIC_SIPI_INTERRUPT (6 << 8)
#define APIC_INIT_INTERRUPT (5 << 8)
#define APIC_DEST_MODE_PHYSICAL (0 << 10)
#define APIC_LEVEL_DEASSERT (0 << 14)
#define APIC_LEVEL_ASSERT (1 << 14)

#define APIC_SEMAPHORE_ADDRESS 0x7100
#define APIC_FUNC_POINTER_ADDRESS 0x7200
#define APIC_GDT_ADDRESS 0x1000


int detect_apic(void);
void enable_x2apic(void);
void x2apic_issue_ipi(size_t destenation, size_t vector, size_t delivery_mode, size_t level);
void x2apic_activate_cpu(size_t cpu_id, void *function_address);