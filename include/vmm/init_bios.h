#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t base_low;
    uint32_t base_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
    uint32_t attributes;
} __attribute__((packed)) e820_mmap_entry_t;

typedef struct
{
    uint16_t count;
    e820_mmap_entry_t entries[];
} __attribute__((packed)) e820_mmap_t;

void init_bios(void);
void get_e820_mmap(e820_mmap_t *mmap_output);
void print_e820_mmap(e820_mmap_t *mmap);