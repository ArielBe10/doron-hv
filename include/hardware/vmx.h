#pragma once

#include <stdint.h>

#define CARRY_FLAG_MASK 1
#define ZERO_FLAG_MASK (1 << 6)

static inline uint64_t vmread(uint64_t field)
{
    uint64_t value;
    asm volatile("vmread %1,%0"
                 : "=rm"(value)
                 : "r"(field));
    return value;
}

static inline void vmwrite(uint64_t field, uint64_t value)
{
    asm volatile("vmwrite %1,%0" ::"r"(field), "r"(value));
}

static inline int vmxon(void *vmxon_address)
{
    uint64_t flags;
    asm volatile("vmxon %1; pushf; pop %0"
                 : "=r"(flags)
                 : "m"(vmxon_address));
    if (flags & CARRY_FLAG_MASK) // invalid vmcs pointer (?)
        return 1;
    if (flags & ZERO_FLAG_MASK) // something else
        return 2;
    return 0;
}

static inline void vmxoff(void)
{
    asm volatile("vmxoff");
}

static inline int vmlaunch(void)
{
    uint64_t flags;
    asm volatile("vmlaunch; pushf; pop %0"
                 : "=r"(flags));
    if (flags & CARRY_FLAG_MASK) // invalid vmcs pointer
        return 1;
    if (flags & ZERO_FLAG_MASK) // something else
        return 2;
    return 0;
}

static inline void vmresume(void)
{
    asm volatile("vmresume");
}

static inline int vmclear(void *vmcs_address)
{
    uint64_t flags;
    asm volatile("vmclear %1; pushf; pop %0"
                 : "=r"(flags)
                 : "m"(vmcs_address));
    if (flags & CARRY_FLAG_MASK) // invalid vmcs pointer
        return 1;
    if (flags & ZERO_FLAG_MASK) // something else
        return 2;
    return 0;
}

static inline int vmptrld(void *vmcs_address)
{
    uint64_t flags;
    asm volatile("vmptrld %1; pushf; pop %0"
                 : "=r"(flags)
                 : "m"(vmcs_address));
    if (flags & CARRY_FLAG_MASK) // invalid vmcs pointer
        return 1;
    if (flags & ZERO_FLAG_MASK) // something else
        return 2;
    return 0;
}

typedef struct {
    uint64_t eptp;
    uint64_t zero;
} invept_descriptor_t;

static inline void invept(uint64_t type, const invept_descriptor_t *descriptor)
{
	asm volatile("invept %1, %0" :: "r"(type), "m"(descriptor));
}