#include "vmm/state.h"

#include "hardware/rsdp.h"
#include "lib/logging.h"


shared_cpu_state_t *create_cpu_states(kheap_metadata_t *kheap) {
    // get cpu count and cpu ids
    int cpu_count;
    rsdp_descriptor_v2_t *rsdp = find_rsdp();
    ASSERT(rsdp != NULL);
    madt_t *madt = (madt_t*)find_sdt_table(rsdp, "APIC");
    ASSERT(madt != NULL);
    cpu_count = get_cpu_count(madt);
    INFO("CPU count: %d", cpu_count);
    uint8_t *cpu_ids = kmalloc(kheap, cpu_count);
    get_cpu_ids(madt, cpu_ids);

    // shared cpu state contains array of pointers to single cpu states
    shared_cpu_state_t *shared_cpu_state = kmalloc(kheap, sizeof(shared_cpu_state_t) + cpu_count * sizeof(single_cpu_state_t *));
    shared_cpu_state->cpu_count = cpu_count;
    
    for (int i = 0; i < cpu_count; i++) {
        single_cpu_state_t *single_cpu_state = kmalloc_aligned(kheap, sizeof(single_cpu_state_t), 0x1000);
        // link shared and single cpu states
        shared_cpu_state->single_cpu_states[i] = single_cpu_state;
        single_cpu_state->shared_cpu_state = shared_cpu_state;

        single_cpu_state->cpu_id = cpu_ids[i];
    }

    return shared_cpu_state;
}