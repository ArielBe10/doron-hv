#include "vmm/vmm.h"
#include "hardware/registers.h"
#include "hardware/vmx.h"
#include "lib/logging.h"

#define CR4_VMX_ENABLE (1 << 13)
#define CR0_NE_ENABLE (1 << 5)
#define IA32_VMX_CR0_FIXED0 0x486
#define IA32_VMX_CR0_FIXED1 0x487
#define IA32_VMX_CR4_FIXED0 0x488
#define IA32_VMX_CR4_FIXED1 0x489
#define IA32_VMX_BASIC 0x480
#define IA32_FEATURE_CONTROL 0x3a

void enter_vmx(kheap_metadata_t *kheap)
{
    char *vmxon_ptr = kmalloc_aligned(kheap, 0x1000, 0x1000);
    char *vmcs_ptr = kmalloc_aligned(kheap, 0x1000, 0x1000);

    DEBUG("allocated vmxon: 0x%p, vmcs: 0x%p", vmxon_ptr, vmcs_ptr);

    *(uint32_t *)vmcs_ptr = rdmsr(IA32_VMX_BASIC);
    *(uint32_t *)vmxon_ptr = rdmsr(IA32_VMX_BASIC);

    wrmsr(IA32_FEATURE_CONTROL, rdmsr(IA32_FEATURE_CONTROL) | 7); // enable execution in SMX and non-SMX operation, and set lock bit

    writecr0((readcr0() | CR0_NE_ENABLE | rdmsr(IA32_VMX_CR0_FIXED0)) & rdmsr(IA32_VMX_CR0_FIXED1));
    writecr4((readcr4() | CR4_VMX_ENABLE | rdmsr(IA32_VMX_CR4_FIXED0)) & rdmsr(IA32_VMX_CR4_FIXED1));

    ASSERT(vmxon(vmxon_ptr) == 0);
    ASSERT(vmclear(vmcs_ptr) == 0);
    ASSERT(vmptrld(vmcs_ptr) == 0);

    INFO("vmx entered successfully");
}
