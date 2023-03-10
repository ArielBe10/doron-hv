#include "vmm/vmexit.h"
#include "hardware/vmcs.h"
#include "hardware/vmx.h"
#include "lib/logging.h"
#include "vmm/int15_hook.h"


int handle_vm_hlt(cpu_state_t *state) {
    uint64_t opcode_len = vmread(VMCS_VM_EXIT_INSTRUCTION_LEN);
    state->registers.rip += opcode_len;
    return 1;
}


int handle_ept_violation(cpu_state_t *state) {
    uint64_t guest_physical_address = vmread(VMCS_GUEST_PHYSICAL_ADDRESS);
    INFO("guest physical address: 0x%p", guest_physical_address);
    return 1;
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
        ERROR("vmcall called from an unknown gate");
        return 1;
    }
}