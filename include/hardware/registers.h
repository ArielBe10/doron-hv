#pragma once

#include <stdint.h>

static inline void wrmsr(uint32_t msr_id, uint64_t msr_value)
{
    uint32_t msr_value_low = msr_value & 0xffffffff, msr_value_high = msr_value >> 32;
    asm volatile("wrmsr"
                 :
                 : "c"(msr_id), "d"(msr_value_high), "a"(msr_value_low));
}

static inline uint64_t rdmsr(uint32_t msr_id)
{
    uint32_t msr_value_high, msr_value_low;
    asm volatile("rdmsr"
                 : "=d"(msr_value_high), "=a"(msr_value_low)
                 : "c"(msr_id));
    uint64_t msr_value = (((uint64_t)msr_value_high) << 32) | msr_value_low;
    return msr_value;
}

static inline uint64_t readcr0(void)
{
    uint64_t cr0;
    asm volatile("mov %%cr0, %0"
                 : "=r"(cr0));
    return cr0;
}

static inline uint64_t readcr2(void)
{
    uint64_t cr2;
    asm volatile("mov %%cr2, %0"
                 : "=r"(cr2));
    return cr2;
}

static inline uint64_t readcr3(void)
{
    uint64_t cr3;
    asm volatile("mov %%cr3, %0"
                 : "=r"(cr3));
    return cr3;
}

static inline uint64_t readcr4(void)
{
    uint64_t cr4;
    asm volatile("mov %%cr4, %0"
                 : "=r"(cr4));
    return cr4;
}

static inline void writecr0(uint64_t cr0)
{
    asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

static inline void writecr2(uint64_t cr2)
{
    asm volatile("mov %0, %%cr2" ::"r"(cr2));
}

static inline void writecr3(uint64_t cr3)
{
    asm volatile("mov %0, %%cr3" ::"r"(cr3));
}

static inline void writecr4(uint64_t cr4)
{
    asm volatile("mov %0, %%cr4" ::"r"(cr4));
}

static inline uint64_t readflags(void)
{
    uint64_t flags;
    asm volatile("pushf; pop %0"
                 : "=r"(flags));
    return flags;
}

static inline void writedr7(uint64_t dr7)
{
    asm volatile("mov %0, %%dr7" ::"r"(dr7));
}

static inline uint64_t readdr7(void)
{
    uint64_t dr7;
    asm volatile("mov %%dr7, %0"
                 : "=r"(dr7));
    return dr7;
}

static inline void cpuid(uint64_t eax, uint64_t ecx, uint64_t *eax_out, uint64_t *ebx_out, uint64_t *ecx_out, uint64_t *edx_out)
{
    asm volatile("cpuid"
                 : "=a"(*eax_out), "=b"(*ebx_out), "=c"(*ecx_out), "=d"(*edx_out)
                 : "a"(eax), "c"(ecx));
}