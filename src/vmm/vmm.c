#include "vmm/vmm.h"
#include "hardware/registers.h"
#include "hardware/vmx.h"
#include "lib/logging.h"
#include "hardware/vmcs.h"
#include "hardware/msr.h"
#include "hardware/exit_reason.h"
#include "lib/string.h"
#include "vmm/vmexit.h"
#include "vmm/bios.h"
#include "hardware/apic.h"

#include <stddef.h>

#define CR4_VMX_ENABLE (1 << 13)
#define CR0_NE_ENABLE (1 << 5)

#define CR4_HOST_REQUIRED1 ((1 << 18) | (1 << 9))

#define MSR_IA32_FEATURE_CONTROL_LOCK_BIT 1
#define MSR_IA32_FEATURE_CONTROL_SMX_BIT 2
#define MSR_IA32_FEATURE_CONTROL_NON_SMX_BIT 4
#define MSR_IA32_DEBUGCTL_NON_RESERVED0 0xffc3

#define RFLAGS_RESERVED1 1
#define RFLAGS_NON_RESERVED0 0x1fffd7

#define VMCS_CS_AR 0xa09b
#define VMCS_DS_AR 0xc093
#define VMCS_TSS_AR 0xc08b  // not present 
#define VMCS_SELECTOR_UNUSABLE (1 << 16)


extern void __vmexit_handler(void);
extern void __vmlaunch_handler(void);
extern void vm_guest_main(void);


typedef enum {
    CPU_STATUS_ACTIVE = 0,
    CPU_STATUS_HLT = 1,
    CPU_STATUS_SHUTDOWN = 2,
    CPU_STATUS_WAIT_FOR_SIPI = 3
} cpu_status_t;


kheap_metadata_t *g_kheap;
cpu_state_t *g_state;


void enter_vmx(void) {
    __vmlaunch_handler();

    // an error occured
    uint64_t error_code = vmread(VMCS_VM_INSTRUCTION_ERROR);
    PANIC("vmlaunch failed: error code %d", error_code);
}


void init_vmx_all_cpus(kheap_metadata_t *kheap, cpu_state_t *state) {
    enable_x2apic();
    cpu_shared_data_t *shared_data = state->cpu_data->shared_data;
    for (size_t index = 0; index < shared_data->cpu_count; index++) {
        size_t cpu_id = shared_data->cpu_states[index]->cpu_data->cpu_id;
        if (cpu_id != state->cpu_data->cpu_id) {
            init_vmx_on_cpu(kheap, shared_data->cpu_states[index]);
        }
    }
    init_vmx_current_cpu(kheap, state);
}


static void init_vmx_current_cpu_wrapper(void) {
    init_vmx_current_cpu(g_kheap, g_state);
}


void init_vmx_on_cpu(kheap_metadata_t *kheap, cpu_state_t *state) {
    g_kheap = kheap;
    g_state = state;
    x2apic_activate_cpu(state->cpu_data->cpu_id, init_vmx_current_cpu_wrapper);
}


void init_vmx_current_cpu(kheap_metadata_t *kheap, cpu_state_t *state)
{
    INFO("init vmx on cpu: %d", state->cpu_data->cpu_id);

    memset(state->cpu_data->vmcs, 0, sizeof(state->cpu_data->vmcs));
    *(uint32_t *)state->cpu_data->vmcs = rdmsr(MSR_IA32_VMX_BASIC);
    *(uint32_t *)state->cpu_data->vmxon = rdmsr(MSR_IA32_VMX_BASIC);

    uint64_t msr_feature_control = rdmsr(MSR_IA32_FEATURE_CONTROL);
    if (msr_feature_control & MSR_IA32_FEATURE_CONTROL_LOCK_BIT) {
        ASSERT(msr_feature_control & MSR_IA32_FEATURE_CONTROL_NON_SMX_BIT); // vmx cannot be set if the msr is locked
    } else {
        wrmsr(MSR_IA32_FEATURE_CONTROL, msr_feature_control | 7); // enable execution in SMX and non-SMX, and set lock bit
    }
    DEBUG("enabled execution in smx and non sxm");

    // should be modified before vmxon
    writecr0((readcr0() | CR0_NE_ENABLE | rdmsr(MSR_IA32_VMX_CR0_FIXED0)) & rdmsr(MSR_IA32_VMX_CR0_FIXED1));
    writecr4((readcr4() | CR4_VMX_ENABLE | rdmsr(MSR_IA32_VMX_CR4_FIXED0)) & rdmsr(MSR_IA32_VMX_CR4_FIXED1));
    wrmsr(MSR_IA32_DEBUGCTL, rdmsr(MSR_IA32_DEBUGCTL) & MSR_IA32_DEBUGCTL_NON_RESERVED0);
	writedr7(readdr7() & 0xffffffff);
    DEBUG("changed registers to support vmx");

    ASSERT(vmxon(state->cpu_data->vmxon) == 0);
    DEBUG("vmxon");

    ASSERT(vmclear(state->cpu_data->vmcs) == 0);
    DEBUG("vmclear");

    ASSERT(vmptrld(state->cpu_data->vmcs) == 0);
    INFO("vmptrld");

    configure_vmcs(state);
    DEBUG("vmcs configured");
}


static uint32_t set_reserved_control_bits(uint32_t control, uint32_t msr, uint32_t true_msr, bool use_true_msr)
{
    uint64_t msr_value = use_true_msr ? rdmsr(true_msr) : rdmsr(msr);
    return (control & (msr_value >> 32)) | (msr_value & 0xffffffff);
}


void configure_vmcs(cpu_state_t *state) {
    uint64_t cr3 = (uint64_t)&state->cpu_data->shared_data->paging_tables.pml4_tables;
    // initialize guest state area
    vmwrite(VMCS_GUEST_CR0, readcr0());
    vmwrite(VMCS_GUEST_CR3, cr3);
    vmwrite(VMCS_GUEST_CR4, readcr4());
    vmwrite(VMCS_GUEST_DR7, readdr7());
    vmwrite(VMCS_GUEST_RSP, 0);  // will be changed to current value of rsp in vmm/vmm.asm
    vmwrite(VMCS_GUEST_RIP, (size_t)vm_guest_main);
    vmwrite(VMCS_GUEST_RFLAGS, (readflags() & RFLAGS_NON_RESERVED0) | RFLAGS_RESERVED1);

    vmwrite(VMCS_GUEST_CS_SELECTOR, get_cs());
    vmwrite(VMCS_GUEST_CS_BASE, 0);
    vmwrite(VMCS_GUEST_CS_LIMIT, 0xffffffff);
    vmwrite(VMCS_GUEST_CS_AR_BYTES, VMCS_CS_AR);

    vmwrite(VMCS_GUEST_DS_SELECTOR, get_ds());
    vmwrite(VMCS_GUEST_DS_BASE, 0);
    vmwrite(VMCS_GUEST_DS_LIMIT, 0xffffffff);
    vmwrite(VMCS_GUEST_DS_AR_BYTES, VMCS_DS_AR);

    vmwrite(VMCS_GUEST_SS_SELECTOR, get_ss());
    vmwrite(VMCS_GUEST_SS_BASE, 0);
    vmwrite(VMCS_GUEST_SS_LIMIT, 0xffffffff);
    vmwrite(VMCS_GUEST_SS_AR_BYTES, VMCS_DS_AR);

    vmwrite(VMCS_GUEST_ES_SELECTOR, get_es());
    vmwrite(VMCS_GUEST_ES_BASE, 0);
    vmwrite(VMCS_GUEST_ES_LIMIT, 0xffffffff);
    vmwrite(VMCS_GUEST_ES_AR_BYTES, VMCS_DS_AR);

    vmwrite(VMCS_GUEST_GS_SELECTOR, get_gs());
    vmwrite(VMCS_GUEST_GS_BASE, 0);
    vmwrite(VMCS_GUEST_GS_LIMIT, 0xffffffff);
    vmwrite(VMCS_GUEST_GS_AR_BYTES, VMCS_DS_AR);

    vmwrite(VMCS_GUEST_FS_SELECTOR, get_fs());
    vmwrite(VMCS_GUEST_FS_BASE, 0);
    vmwrite(VMCS_GUEST_FS_LIMIT, 0xffffffff);
    vmwrite(VMCS_GUEST_FS_AR_BYTES, VMCS_DS_AR);

    vmwrite(VMCS_GUEST_TR_SELECTOR, get_ds());  // the tr actually points to a ds, may cause a bug
    vmwrite(VMCS_GUEST_TR_BASE, 0);
    vmwrite(VMCS_GUEST_TR_LIMIT, 0xffffffff);
    vmwrite(VMCS_GUEST_TR_AR_BYTES, VMCS_TSS_AR);

    gdtr_t gdtr = get_gdtr();
    idtr_t idtr = get_idtr();
    vmwrite(VMCS_GUEST_GDTR_BASE, gdtr.address);
    vmwrite(VMCS_GUEST_GDTR_LIMIT, gdtr.limit);
    vmwrite(VMCS_GUEST_IDTR_BASE, idtr.address);
    vmwrite(VMCS_GUEST_IDTR_LIMIT, idtr.limit);

    vmwrite(VMCS_GUEST_LDTR_SELECTOR, 0);  // null selector should be marked unusable
    vmwrite(VMCS_GUEST_LDTR_BASE, 0);
    vmwrite(VMCS_GUEST_LDTR_LIMIT, 0xff);
    vmwrite(VMCS_GUEST_LDTR_AR_BYTES, VMCS_SELECTOR_UNUSABLE);

    vmwrite(VMCS_GUEST_EFER, rdmsr(MSR_IA32_EFER));
    vmwrite(VMCS_GUEST_IA32_DEBUGCTL, rdmsr(MSR_IA32_DEBUGCTL) & 0xffffffff);
    vmwrite(VMCS_GUEST_IA32_DEBUGCTL_HIGH, rdmsr(MSR_IA32_DEBUGCTL) >> 32);

    vmwrite(VMCS_GUEST_SYSENTER_CS, 8);
    vmwrite(VMCS_GUEST_SYSENTER_EIP, 0xffff);
    vmwrite(VMCS_GUEST_SYSENTER_ESP, 0xffff);

    vmwrite(VMCS_GUEST_ACTIVITY_STATE, CPU_STATUS_ACTIVE);
    vmwrite(VMCS_GUEST_INTERRUPTIBILITY_INFO, 0);  // default: not blocked by sti, mov ss, smi, etc
    vmwrite(VMCS_VMCS_LINK_POINTER, -1ULL);  // should be set to -1 if vmcs shadowing is disabled
    // vmx preemptive time value?
    // pdptes?
    // guest interrupt status?
    // PML index?

    // initialize host state area
    vmwrite(VMCS_HOST_CR0, readcr0());
    vmwrite(VMCS_HOST_CR3, cr3);
    vmwrite(VMCS_HOST_CR4, readcr4());  // according to HyperWin, should be or'ed with CR4_HOST_REQUIRED1 but I didn't find it in the docs
    vmwrite(VMCS_HOST_RIP, (size_t)__vmexit_handler);
    vmwrite(VMCS_HOST_RSP, (size_t)state->stack + sizeof(state->stack));  // from high addresses to lower
    vmwrite(VMCS_HOST_EFER, rdmsr(MSR_IA32_EFER));

    DEBUG("cs: %d, ds: %d", get_cs(), get_ds());
    ASSERT(((get_cs() & 7) == 0) && ((get_ds() & 7) == 0));  // TI and RPL sould be 0
    vmwrite(VMCS_HOST_CS_SELECTOR, get_cs());
    vmwrite(VMCS_HOST_DS_SELECTOR, get_ds());
    vmwrite(VMCS_HOST_ES_SELECTOR, get_es());
    vmwrite(VMCS_HOST_SS_SELECTOR, get_ss());
    vmwrite(VMCS_HOST_GS_SELECTOR, get_gs());
    vmwrite(VMCS_HOST_FS_SELECTOR, get_fs()); 
    vmwrite(VMCS_HOST_TR_SELECTOR, get_ds());  // probably won't cause a bug 
    vmwrite(VMCS_HOST_TR_BASE, get_ds());
    vmwrite(VMCS_HOST_GDTR_BASE, (size_t)state->cpu_data->shared_data->gdt);
    vmwrite(VMCS_HOST_GDTR_BASE, 0);

    // fs and gs are available and not used by the hardware
    vmwrite(VMCS_HOST_FS_BASE, (size_t)state);
    vmwrite(VMCS_HOST_GS_BASE, 0);
    
    vmwrite(VMCS_HOST_SYSENTER_CS, 0xff);
    vmwrite(VMCS_HOST_SYSENTER_EIP, 0xffffffff);
    vmwrite(VMCS_HOST_SYSENTER_ESP, 0xffffffff);

    // initialize vm execution control
    uint64_t use_true_msr = (rdmsr(MSR_IA32_VMX_BASIC) & (1UL << 55)) >> 55;  // bit 55 of MSR_IA32_VMX_BASIC
    DEBUG("IA32_VMX_BASIC_MSR: %p, bit 55 is %d", rdmsr(MSR_IA32_VMX_BASIC), use_true_msr);

    uint32_t pin_based_vmx_control = set_reserved_control_bits(0, MSR_IA32_VMX_PINBASED_CTLS, MSR_IA32_VMX_TRUE_PINBASED_CTLS, use_true_msr);
    DEBUG("pin based vmx control: %p", pin_based_vmx_control);
    vmwrite(VMCS_PIN_BASED_VM_EXEC_CONTROL, pin_based_vmx_control);

    uint32_t cpu_based_vmx_control = set_reserved_control_bits(CPU_BASED_HLT_EXITING | CPU_BASED_ACTIVATE_SECONDARY_CONTROLS |
        CPU_BASED_ACTIVATE_MSR_BITMAP,
        MSR_IA32_VMX_PROCBASED_CTLS, MSR_IA32_VMX_TRUE_PROCBASED_CTLS, use_true_msr);
    DEBUG("cpu based vmx control: %p", cpu_based_vmx_control);
    vmwrite(VMCS_CPU_BASED_VM_EXEC_CONTROL, cpu_based_vmx_control);

    uint32_t secondary_cpu_based_vmx_control = set_reserved_control_bits(CPU_BASED_CTL2_ENABLE_EPT | CPU_BASED_CTL2_UNRESTRICTED_GUEST,
        MSR_IA32_VMX_PROCBASED_CTLS2, 0, 0);  // for this control always use MSR_IA32_VMX_PROCBASED_CTLS2
    DEBUG("secondary cpu based vmx control: %p", secondary_cpu_based_vmx_control);
    vmwrite(VMCS_SECONDARY_VM_EXEC_CONTROL, secondary_cpu_based_vmx_control);

    // do not break on any exception
    vmwrite(VMCS_EXCEPTION_BITMAP, 0);
    vmwrite(VMCS_PAGE_FAULT_ERROR_CODE_MASK, 0);
    vmwrite(VMCS_PAGE_FAULT_ERROR_CODE_MATCH, 0);

    vmwrite(VMCS_TSC_OFFSET, 0);  // disabled in cpu based execution controls
    vmwrite(VMCS_TSC_OFFSET_HIGH, 0);
    vmwrite(VMCS_CR0_GUEST_HOST_MASK, 0);  // guest can read & write cr0/4
    vmwrite(VMCS_CR4_GUEST_HOST_MASK, 0);
    vmwrite(VMCS_CR3_TARGET_COUNT, 0);  // disable cr3 target controls
    // io bitmap is disabled and doesn't need to be initialized 

    vmwrite(VMCS_EPT_POINTER, state->cpu_data->shared_data->ept_paging_tables.eptp);
    vmwrite(VMCS_MSR_BITMAP, (size_t)&state->cpu_data->shared_data->msr_bitmap[0]);

    // initialize vm exit control 
    uint32_t vmx_exit_control = set_reserved_control_bits(VM_EXIT_SAVE_EFER | VM_EXIT_LOAD_EFER | VM_EXIT_IA32E_MODE,
        MSR_IA32_VMX_EXIT_CTLS, MSR_IA32_VMX_TRUE_EXIT_CTLS, use_true_msr);
    DEBUG("vmx exit control: %p", vmx_exit_control);
    vmwrite(VMCS_VM_EXIT_CONTROLS, vmx_exit_control);
    vmwrite(VMCS_VM_EXIT_MSR_STORE_COUNT, 0);
    vmwrite(VMCS_VM_EXIT_MSR_LOAD_COUNT, 0);
    
    // initialize vm entry control
    uint32_t vmx_entry_control = set_reserved_control_bits(VM_ENTRY_LOAD_EFER | VM_ENTRY_IA32E_MODE,
        MSR_IA32_VMX_ENTRY_CTLS, MSR_IA32_VMX_TRUE_ENTRY_CTLS, use_true_msr);
    DEBUG("vmx entry control: %p", vmx_entry_control);
    vmwrite(VMCS_VM_ENTRY_CONTROLS, vmx_entry_control);
    vmwrite(VMCS_VM_ENTRY_MSR_LOAD_COUNT, 0);
    vmwrite(VMCS_VM_ENTRY_INTR_INFO, 0);

    // vmwrite(VMCS_VIRTUAL_PROCESSOR_ID, 1);
}


void vmexit_handler(void) {
    uint64_t exit_reason = vmread(VMCS_VM_EXIT_REASON) & VMCS_VM_EXIT_REASON_MASK;
    INFO("vmexit: %s", EXIT_REASON_NAMES[exit_reason]);

    cpu_state_t *state = (cpu_state_t*)vmread(VMCS_HOST_FS_BASE);

    int status;
    switch (exit_reason) {
        case EXIT_REASON_HLT:
            status = handle_vm_hlt(state);
            break;
        case EXIT_REASON_EPT_VIOLATION:
            status = handle_ept_violation(state);
            break;
        case EXIT_REASON_VMCALL:
            status = handle_vmcall(state);
            break;
        default:
            PANIC("unsupported exit reason: %s", EXIT_REASON_NAMES[exit_reason]);
    }
    if (status != 0) {
        PANIC("error while handling vmexit: %d", status);
    }
    INFO("VMRESUME");
}
