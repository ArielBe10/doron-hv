#include "vmm/vmm.h"
#include "hardware/registers.h"
#include "hardware/vmx.h"
#include "lib/logging.h"
#include "hardware/vmcs.h"
#include "hardware/msr.h"

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


typedef enum {
    CPU_STATE_ACTIVE = 0,
    CPU_STATE_HLT = 1,
    CPU_STATE_SHUTDOWN = 2,
    CPU_STATE_WAIT_FOR_SIPI = 3
} cpu_state_t;


void enter_vmx(kheap_metadata_t *kheap, single_cpu_state_t *state)
{
    INFO("entering vmx");

    *(uint32_t *)state->vmcs = rdmsr(MSR_IA32_VMX_BASIC);
    *(uint32_t *)state->vmxon = rdmsr(MSR_IA32_VMX_BASIC);

    uint64_t msr_feature_control = rdmsr(MSR_IA32_FEATURE_CONTROL);
    if (msr_feature_control & MSR_IA32_FEATURE_CONTROL_LOCK_BIT) {
        ASSERT(msr_feature_control & MSR_IA32_FEATURE_CONTROL_NON_SMX_BIT); // vmx cannot be set if the msr is locked
    } else {
        wrmsr(MSR_IA32_FEATURE_CONTROL, msr_feature_control | 7); // enable execution in SMX and non-SMX, and set lock bit
    }
    DEBUG("enabled execution in smx and non sxm");

    writecr0((readcr0() | CR0_NE_ENABLE | rdmsr(MSR_IA32_VMX_CR0_FIXED0)) & rdmsr(MSR_IA32_VMX_CR0_FIXED1));
    writecr4((readcr4() | CR4_VMX_ENABLE | rdmsr(MSR_IA32_VMX_CR4_FIXED0)) & rdmsr(MSR_IA32_VMX_CR4_FIXED1));

    DEBUG("changed cr0 and cr4 to support vmx");

    ASSERT(vmxon(state->vmxon) == 0);
    DEBUG("vmxon");

    ASSERT(vmclear(state->vmcs) == 0);
    DEBUG("vmclear");

    configure_vmcs(state);
    DEBUG("vmcs configured");

    ASSERT(vmptrld(state->vmcs) == 0);
    INFO("vmx entered successfully");

    vmlaunch();
}


static uint32_t set_reserved_control_bits(uint32_t control, uint32_t msr)
{
    uint64_t msr_value = rdmsr(msr);
    return (control & (msr_value >> 32)) | (msr_value & 0xffffffff);
}


void configure_vmcs(single_cpu_state_t *state) {
    // should be modified before vmxon
    wrmsr(MSR_IA32_DEBUGCTL, rdmsr(MSR_IA32_DEBUGCTL) & MSR_IA32_DEBUGCTL_NON_RESERVED0);
	writedr7(readdr7() & 0xffffffff);
    vmwrite(VMCS_GUEST_SYSENTER_EIP, 0xffff);
    vmwrite(VMCS_GUEST_SYSENTER_ESP, 0xffff);

    // initialize guest state area
    vmwrite(VMCS_GUEST_CR0, readcr0());
    vmwrite(VMCS_GUEST_CR3, readcr3());
    vmwrite(VMCS_GUEST_CR4, readcr4());
    vmwrite(VMCS_GUEST_DR7, readdr7());
    vmwrite(VMCS_GUEST_RSP, (size_t)state->stack + sizeof(state->stack)); // Will be handled before vmlaunch is called, see x86_64.asm
    vmwrite(VMCS_GUEST_RIP, (size_t)vmenter_handler);
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

    vmwrite(VMCS_GUEST_ACTIVITY_STATE, CPU_STATE_ACTIVE);
    vmwrite(VMCS_GUEST_INTERRUPTIBILITY_INFO, 0);  // default: not blocked by sti, mov ss, smi, etc
    vmwrite(VMCS_VMCS_LINK_POINTER, -1ULL);  // should be set to -1 if vmcs shadowing is disabled
    // vmx preemptive time value?
    // pdptes?
    // guest interrupt status?
    // PML index?

    // initialize host state area
    vmwrite(VMCS_HOST_CR0, readcr0());
    vmwrite(VMCS_HOST_CR3, readcr3());  // todo: create new paging tables
    vmwrite(VMCS_HOST_CR4, readcr4() | CR4_HOST_REQUIRED1);
    vmwrite(VMCS_HOST_RIP, (size_t)vmexit_handler);
    vmwrite(VMCS_HOST_RSP, (size_t)state->stack + sizeof(state->stack));  // from high addresses to lower
    vmwrite(VMCS_HOST_EFER, rdmsr(MSR_IA32_EFER));

    vmwrite(VMCS_HOST_CS_SELECTOR, get_cs());
    vmwrite(VMCS_HOST_DS_SELECTOR, get_ds());
    vmwrite(VMCS_HOST_ES_SELECTOR, get_es());
    vmwrite(VMCS_HOST_SS_SELECTOR, get_ss());
    vmwrite(VMCS_HOST_GS_SELECTOR, get_gs());
    vmwrite(VMCS_HOST_FS_SELECTOR, get_fs()); 
    vmwrite(VMCS_HOST_TR_SELECTOR, get_ds());  // probably ok
    vmwrite(VMCS_HOST_TR_BASE, get_ds());
    vmwrite(VMCS_HOST_GDTR_BASE, (size_t)state->gdt);

    // fs and gs are available and not used by the hardware
    vmwrite(VMCS_HOST_FS_BASE, (size_t)state);
    vmwrite(VMCS_HOST_GS_BASE, 0);
    
    vmwrite(VMCS_HOST_SYSENTER_CS, 0xff);
    // should be canonical addresses
    vmwrite(VMCS_HOST_SYSENTER_EIP, 0xffffffff);
    vmwrite(VMCS_HOST_SYSENTER_ESP, 0xffffffff);


    vmwrite(VMCS_GUEST_SYSENTER_CS, 8);

    // initialize vm execution control
    vmwrite(VMCS_PIN_BASED_VM_EXEC_CONTROL, set_reserved_control_bits(0, MSR_IA32_VMX_PINBASED_CTLS));

    // vmwrite(VMCS_CPU_BASED_VM_EXEC_CONTROL, set_reserved_control_bits(
    //     CPU_BASED_ACTIVATE_MSR_BITMAP | CPU_BASED_ACTIVATE_SECONDARY_CONTROLS, MSR_IA32_VMX_PROCBASED_CTLS));
    vmwrite(VMCS_CPU_BASED_VM_EXEC_CONTROL, set_reserved_control_bits(0, MSR_IA32_VMX_PROCBASED_CTLS));  // todo: enable msr bitmap & secondary vm execution controls
    
    // vmwrite(VMCS_SECONDARY_VM_EXEC_CONTROL, set_reserved_control_bits(
    //     CPU_BASED_CTL2_ENABLE_INVPCID | CPU_BASED_CTL2_RDTSCP  | CPU_BASED_CTL2_ENABLE_EPT | CPU_BASED_CTL2_UNRESTRICTED_GUEST, MSR_IA32_VMX_PROCBASED_CTLS2));

    // do not break on any exception
    vmwrite(VMCS_EXCEPTION_BITMAP, 0);
    vmwrite(VMCS_PAGE_FAULT_ERROR_CODE_MASK, 0);
    vmwrite(VMCS_PAGE_FAULT_ERROR_CODE_MATCH, 0);

    // io bitmap is disabled

    vmwrite(VMCS_TSC_OFFSET, 0);  // disabled in cpu based execution controls
    vmwrite(VMCS_TSC_OFFSET_HIGH, 0);

    vmwrite(VMCS_CR0_GUEST_HOST_MASK, 0);  // guest can read & write to cr0/4
    vmwrite(VMCS_CR4_GUEST_HOST_MASK, 0);

    vmwrite(VMCS_CR3_TARGET_COUNT, 0);  // disable cr3 target controls
    // todo: add msr bitmap and ept

    // initialize vm exit control 
    vmwrite(VMCS_VM_EXIT_CONTROLS, set_reserved_control_bits(VM_EXIT_SAVE_EFER | VM_EXIT_LOAD_EFER | VM_EXIT_IA32E_MODE, MSR_IA32_VMX_EXIT_CTLS));
    vmwrite(VMCS_VM_EXIT_MSR_STORE_COUNT, 0);
    vmwrite(VMCS_VM_EXIT_MSR_LOAD_COUNT, 0);
    
    // initialize vm entry control
    vmwrite(VMCS_VM_ENTRY_CONTROLS, set_reserved_control_bits(VM_ENTRY_LOAD_EFER | VM_ENTRY_IA32E_MODE, MSR_IA32_VMX_ENTRY_CTLS));
    vmwrite(VMCS_VM_ENTRY_MSR_LOAD_COUNT, 0);
    vmwrite(VMCS_VM_ENTRY_INTR_INFO, 0);

    vmwrite(VMCS_VIRTUAL_PROCESSOR_ID, 1);
}


void vmexit_handler(void) {
    INFO("VMEXIT");
}


void vmenter_handler(void) {
    INFO("VMENTER");
}