#include "hardware/apic.h"
#include "hardware/registers.h"
#include "hardware/msr.h"
#include "vmm/paging.h"
#include "lib/logging.h"
#include "vmm/bios.h"
#include "lib/string.h"


#define CPUID_2XAPIC_AVAILABLE (1 << 21)
#define MSR_IA32_APIC_BASE_X2APIC_ENABLE ((1 << 10) | (1 << 11))
#define APIC_DESTINAION_BIT_OFFSET 32


extern void apic_start(void);
extern void real_mode_code_start_offset(void);


int detect_apic(void) {
    uint64_t tmp, ecx;
    cpuid(1, 0, &tmp, &tmp, &ecx, &tmp);
    return (ecx & CPUID_2XAPIC_AVAILABLE) ? 0 : 1;
}


void enable_x2apic(void) {
    ASSERT(detect_apic() == 0);
    wrmsr(MSR_IA32_APIC_BASE, rdmsr(MSR_IA32_APIC_BASE) | MSR_IA32_APIC_BASE_X2APIC_ENABLE);
    DEBUG("x2apic enabled");
}


void x2apic_issue_ipi(size_t destenation, size_t vector, size_t delivery_mode, size_t level) {
    wrmsr(MSR_IA32_X2APIC_ICR, (destenation << APIC_DESTINAION_BIT_OFFSET) | delivery_mode | vector | level);
}


static void sleep(void) {
    for (size_t i = 0; i < 0xffffff; i++) {}
}


void x2apic_activate_cpu(size_t cpu_id, void *function_address) {
    INFO("activating cpu: %d, function address: %p", cpu_id, function_address);

    *(uint8_t *)APIC_SEMAPHORE_ADDRESS = 0;
    *(size_t *)APIC_FUNC_POINTER_ADDRESS = (size_t)function_address;

    // copy gdt
    gdtr_t gdtr = get_gdtr();
    memcpy((void *)APIC_GDT_ADDRESS, (void *)gdtr.address, gdtr.limit + 1);

    size_t apic_start_copy_address = REAL_MODE_CODE_START_ADDRESS + (size_t)apic_start - (size_t)real_mode_code_start_offset;
    ASSERT(apic_start_copy_address % PAGE_SIZE == 0);

    sleep();
    x2apic_issue_ipi(cpu_id, 0, APIC_INIT_INTERRUPT, APIC_LEVEL_DEASSERT);
    sleep();
    DEBUG("sent INIT to cpu: %d", cpu_id);

    sleep();
    x2apic_issue_ipi(cpu_id, apic_start_copy_address / PAGE_SIZE, APIC_SIPI_INTERRUPT, APIC_LEVEL_ASSERT);
    sleep();
    DEBUG("sent SIPI to cpu: %d", cpu_id);

    DEBUG("waiting for cpu: %d", cpu_id);
    while (*(uint8_t*)APIC_SEMAPHORE_ADDRESS == 0) {}  // wait for other cpu
    sleep();

    INFO("activated cpu: %d", cpu_id);
}