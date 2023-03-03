#include "vmm/vmexit.h"
#include "hardware/vmcs.h"
#include "hardware/vmx.h"


int handle_vm_hlt(cpu_state_t *state) {
    uint64_t opcode_len;
    vmread(VMCS_VM_EXIT_INSTRUCTION_LEN, &opcode_len);
    state->registers.rip += opcode_len;
    return 0;
}