#include "vmm/state.h"

#include "hardware/rsdp.h"
#include "lib/logging.h"
#include "hardware/registers.h"
#include "lib/string.h"
#include "vmm/paging.h"


cpu_shared_data_t *create_cpu_data(kheap_metadata_t *kheap) {
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

    // shared data contains array of pointers to cpu states
    cpu_shared_data_t *shared_data = kmalloc_aligned(kheap, sizeof(cpu_shared_data_t) + cpu_count * sizeof(cpu_state_t *), PAGE_SIZE);
    shared_data->cpu_count = cpu_count;
    
    for (int i = 0; i < cpu_count; i++) {
        cpu_state_t *state = kmalloc(kheap, sizeof(cpu_state_t));
        // link shared data cpu states
        state->cpu_data = kmalloc_aligned(kheap, sizeof(cpu_data_t), PAGE_SIZE);
        state->cpu_data->cpu_id = cpu_ids[i];
        state->cpu_data->shared_data = shared_data;
        shared_data->cpu_states[i] = state;
    }

    return shared_data;
}


void set_cpu_data(cpu_shared_data_t *shared_data) {
    DEBUG("initializing cpu data at address: %p", shared_data);
    gdtr_t gdtr = get_gdtr();
    memcpy(&shared_data->gdt, (void *)gdtr.address, gdtr.limit + 1);

    DEBUG("creating paging tables");
    create_paging_tables(&shared_data->paging_tables);

    DEBUG("creating ept paging tables");
    create_ept_paging_tables(&shared_data->ept_paging_tables);

    DEBUG("finished initializing cpu data");
}
