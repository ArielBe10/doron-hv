#pragma once

#include <stdint.h>

static inline uint8_t inb(uint16_t port)
{
    uint8_t result;
    __asm__("in %%dx, %%al"
            : "=a"(result)
            : "d"(port));
    return result;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t result;
    __asm__("in %%dx, %%ax"
            : "=a"(result)
            : "d"(port));
    return result;
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t result;
    __asm__("in %%dx, %%eax"
            : "=a"(result)
            : "d"(port));
    return result;
}

static inline void outb(uint16_t port, uint8_t data)
{
    __asm__("out %%al, %%dx"
            :
            : "a"(data), "d"(port));
}

static inline void outw(uint16_t port, uint16_t data)
{
    __asm__("out %%ax, %%dx"
            :
            : "a"(data), "d"(port));
}

static inline void outl(uint16_t port, uint32_t data)
{
    __asm__("out %%eax, %%dx"
            :
            : "a"(data), "d"(port));
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}