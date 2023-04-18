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

static inline uint16_t get_cs(void)
{
    uint16_t cs;
    asm volatile("mov %%cs, %0": "=r"(cs));
    return cs;
}

static inline uint16_t get_ds(void)
{
    uint16_t ds;
    asm volatile("mov %%ds, %0": "=r"(ds));
    return ds;
}

static inline uint16_t get_ss(void)
{
    uint16_t ss;
    asm volatile("mov %%ss, %0": "=r"(ss));
    return ss;
}

static inline uint16_t get_es(void)
{
    uint16_t es;
    asm volatile("mov %%es, %0": "=r"(es));
    return es;
}

static inline uint16_t get_fs(void)
{
    uint16_t fs;
    asm volatile("mov %%fs, %0": "=r"(fs));
    return fs;
}

static inline uint16_t get_gs(void)
{
    uint16_t gs;
    asm volatile("mov %%gs, %0": "=r"(gs));
    return gs;
}

static inline uint64_t get_rsp(void)
{
    uint64_t rsp;
    asm volatile("mov %%rsp, %0": "=r"(rsp));
    return rsp;
}

typedef struct
{
    uint16_t limit;
    uint64_t address;
} __attribute__((__packed__)) gdtr_t;

typedef struct
{
    uint16_t limit;
    uint64_t address;
} __attribute__((__packed__)) idtr_t;


static inline gdtr_t get_gdtr(void)
{
    gdtr_t gdtr = {.address = 0, .limit = 0};
    asm volatile("sgdt %0"::"m"(gdtr));
    return gdtr;
}

static inline idtr_t get_idtr(void)
{
    idtr_t idtr = {.address = 0, .limit = 0};
    asm volatile("sidt %0"::"m"(idtr));
    return idtr;
}