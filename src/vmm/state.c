#include "vmm/state.h"

#include "hardware/rsdp.h"
#include "lib/logging.h"
#include "hardware/registers.h"
#include "lib/string.h"
#include "vmm/paging.h"
#include "vmm/kheap.h"
#include "vmm/int15_hook.h"

extern void __hypervisor_start(void);
extern void __hypervisor_end(void);


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


void set_cpu_data(cpu_shared_data_t *shared_data, kheap_metadata_t kheap) {
    DEBUG("initializing cpu data at address: %p", shared_data);
    
    // create page tables and ept tables
    gdtr_t gdtr = get_gdtr();
    memcpy(&shared_data->gdt, (void *)gdtr.address, gdtr.limit + 1);
    create_paging_tables(&shared_data->paging_tables);
    create_ept_paging_tables(&shared_data->ept_paging_tables);

    // create fake mmap to be a copy of the real one
    // allocate extra memory since regions may be added later
    shared_data->fake_mmap = kmalloc(&kheap, sizeof(e820_mmap_t) + MAX_MMAP_ENTRIES * sizeof(e820_mmap_entry_t));
    memcpy(shared_data->fake_mmap, kheap.mmap, sizeof(e820_mmap_t) + kheap.mmap->count * sizeof(e820_mmap_entry_t));

    size_t hypervisor_start = ALIGNED_DOWN((size_t)__hypervisor_start, PAGE_SIZE);
    size_t hypervisor_end = ALIGNED_UP((size_t)__hypervisor_end, PAGE_SIZE);
    size_t hypervisor_length = hypervisor_end - hypervisor_start;

    DEBUG("reserved hypervisor memory from %p to %p", hypervisor_start, hypervisor_end);
    // update_ept_access_rights(&shared_data->ept_paging_tables, hypervisor_start, hypervisor_length, 0);
    ASSERT(mark_mmap_memory_unuseable(shared_data->fake_mmap, hypervisor_start, hypervisor_length) == 0);

    memset(shared_data->msr_bitmap, 0, 0x1000);

    DEBUG("finished initializing cpu data");
}
