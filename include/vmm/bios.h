#pragma once

#include <stdint.h>

#define MMAP_AVAILABLE 1
#define REAL_MODE_CODE_START_ADDRESS 0x3000  // should be page aligned for APIC start


typedef struct
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t attributes;
} __attribute__((packed)) e820_mmap_entry_t;

typedef struct
{
    uint16_t count;
    e820_mmap_entry_t entries[];
} __attribute__((packed)) e820_mmap_t;

typedef struct
{
    uint8_t size;           // always 16
    uint8_t zero;           // always 0
    uint16_t sectors_count; // max 127 (sometimes)
    uint16_t dst_offset;
    uint16_t dst_segment;
    uint64_t lba_first_sector;
} __attribute__((packed)) bios_dap_t;

void init_bios(void);
void get_e820_mmap(e820_mmap_t *mmap_output);
void print_e820_mmap(const e820_mmap_t *mmap);
void read_disk(bios_dap_t *dap, uint8_t drive_number);
void os_entry(void);