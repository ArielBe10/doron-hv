#include "vmm/vmexit.h"
#include "hardware/vmcs.h"
#include "hardware/vmx.h"
#include "lib/logging.h"
#include "vmm/int15_hook.h"
#include "demo/syscall.h"
#include "vmm/msr_hook.h"
#include "hardware/msr.h"
#include "hardware/registers.h"


#define VMCALL_REASON_INIT 0x1234


static void next_rip(cpu_state_t *state) {
    uint64_t opcode_len = vmread(VMCS_VM_EXIT_INSTRUCTION_LEN);
    state->registers.rip += opcode_len;
}


int handle_vm_hlt(cpu_state_t *state) {
    next_rip(state);
    return 1;
}


int handle_ept_violation(cpu_state_t *state) {
    uint64_t guest_physical_address = vmread(VMCS_GUEST_PHYSICAL_ADDRESS);
    INFO("guest physical address: 0x%p", guest_physical_address);
    return 1;
}


void init_hv(cpu_state_t *state) {
    // block msr writes
    ASSERT(update_msr_access_policy(state->cpu_data->shared_data, MSR_IA32_SYSENTER_EIP, 0, 1) == 0);
    ASSERT(update_msr_access_policy(state->cpu_data->shared_data, MSR_IA32_LSTAR, 0, 1) == 0);

    size_t ssdt_start = ALIGNED_DOWN((uint64_t)&ssdt, PAGE_SIZE);
    size_t ssdt_length = ALIGNED_UP((size_t)&ssdt + sizeof(ssdt_t) - ssdt_start, PAGE_SIZE);
    update_ept_access_rights(&state->cpu_data->shared_data->ept_paging_tables, ssdt_start, ssdt_length, EPT_READ);
}


int handle_vmcall(cpu_state_t *state) {
    if (state->registers.rip == VMCALL_INT15_GATE) {
        if (state->registers.rax == 0xe820) {  // get memory map flag
            fake_int15_handler(state);
            return 0;
        } else {
            state->registers.rip = state->cpu_data->shared_data->int15_offset;
            vmwrite(VMCS_GUEST_CS_BASE, state->cpu_data->shared_data->int15_segment);
            return 0;
        }
    } else {
        uint64_t vmcall_reason = state->registers.rax;
        switch (vmcall_reason) {
            case VMCALL_REASON_INIT:
                init_hv(state);
                next_rip(state);
                return 0;
            default:
                ERROR("unknown vmcall reason: %p", vmcall_reason);
                return 1;
        }
    }
}