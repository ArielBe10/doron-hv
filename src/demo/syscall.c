#include "demo/syscall.h"
#include "hardware/registers.h"
#include "hardware/msr.h"
#include "lib/logging.h"
#include "lib/string.h"
#include "stddef.h"


#define MSR_IA32_EFER_SCE 1
#define CODESEG64_USERMODE 0x18  // should match src/boot/gdt.asm

#define KERNEL_STACK_SIZE 0x1000

uint8_t kernel_stack[KERNEL_STACK_SIZE];


extern void __syscall_handler(uint64_t syscall_number);



static void __sysenter_handler(uint64_t syscall_number) {
    syscall_handler(syscall_number);

    uint64_t rip = *(uint64_t*)sysenter_rsp;
    void (*ret)(void) = (void*)rip;
    ret();
}


void syscall_handler(uint64_t syscall_number) {
    DEBUG("SYSCALL: 0x%x", syscall_number);

    if (syscall_number > SSDT_LENGTH) {
        ERROR("invalid syscall number: 0x%x", syscall_number);
    }
    else if (ssdt.entries[syscall_number] == NULL) {
        ERROR("unsupported syscall: 0x%x", syscall_number);
    }
    else {
        void (*handler)(void) = (void*)(ssdt.entries[syscall_number]);
        handler();
    }
}

void init_ssdt(void) {
    INFO("init ssdt");
    wrmsr(MSR_IA32_LSTAR, (uint64_t)__syscall_handler);
    wrmsr(MSR_IA32_STAR, (((uint64_t)get_cs()) << 32) | (((uint64_t)CODESEG64_USERMODE) << 48));  // stack descriptors should be right after code descriptors
    DEBUG("MSR_IA32_STAR: %p", rdmsr(MSR_IA32_STAR));
    wrmsr(MSR_IA32_EFER, rdmsr(MSR_IA32_EFER) | MSR_IA32_EFER_SCE);  // enable syscalls

    wrmsr(MSR_IA32_SYSENTER_CS, get_cs());
    wrmsr(MSR_IA32_SYSENTER_EIP, (uint64_t)__sysenter_handler);
    wrmsr(MSR_IA32_SYSENTER_ESP, (uint64_t)&kernel_stack);

    memset((void*)&ssdt, 0, sizeof(ssdt_t));
}


void setup_syscall_handler(uint64_t syscall_number, syscall_handler_t handler) {
    ASSERT(syscall_number < SSDT_LENGTH);
    ssdt.entries[syscall_number] = handler;
}
