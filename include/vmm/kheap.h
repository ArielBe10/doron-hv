#pragma once

#include "vmm/init_bios.h"
#include <stdint.h>

typedef struct
{
    uint64_t end;
    e820_mmap_t *mmap;
} kheap_metadata_t;

kheap_metadata_t setup_kheap(e820_mmap_t *mmap, uint64_t max_size);

void *kmalloc(kheap_metadata_t *kheap, uint64_t size);