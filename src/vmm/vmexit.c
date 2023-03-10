#include "vmm/vmexit.h"
#include "hardware/vmcs.h"
#include "hardware/vmx.h"
#include "lib/logging.h"


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
