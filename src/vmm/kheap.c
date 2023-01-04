#include "vmm/kheap.h"
#include "lib/logging.h"
#include <stdint.h>

#define MMAP_AVAILABLE 1
#define PAGE_ALIGNED(addr) ((addr) & (~0xfff))

kheap_metadata_t setup_kheap(e820_mmap_t *mmap, uint64_t max_size)
{
    kheap_metadata_t kheap;
    kheap.mmap = mmap;

    // find the last available & large enough mmap entry
    for (int mmap_entry_index = mmap->count - 1; mmap_entry_index >= 0; mmap_entry_index--)
    {
        e820_mmap_entry_t entry = mmap->entries[mmap_entry_index];
        if (entry.type == MMAP_AVAILABLE && entry.length >= max_size)
        {
            kheap.end = PAGE_ALIGNED(entry.base + 0x1000); // avoid unavailable pages
            break;
        }
    }
    DEBUG("kheap created at address 0x%p", kheap.end);
    return kheap;
}

void *kmalloc(kheap_metadata_t *kheap, uint64_t size)
{
    void *address = (void *)kheap->end;
    kheap->end += size;
    return address;
}