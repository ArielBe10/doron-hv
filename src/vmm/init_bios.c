#include "vmm/init_bios.h"
#include "lib/logging.h"
#include "lib/string.h"

#define REAL_MODE_CODE_START_ADDRESS 0x3000
#define E820_MMAP_OUTPUT_ADDRESS 0x1000 // match MMAP_OUTPUT_ADDRESS in src/bios/mmap.asm

extern void real_mode_code_start_offset(void);
extern void real_mode_code_end_offset(void);
extern void call_real_mode_function_wrapper(void *);

extern void bios_mmap(void);

void init_bios(void)
{
    DEBUG("init bios");
    char *code_src_address = (char *)real_mode_code_start_offset;
    char *code_dst_address = (char *)REAL_MODE_CODE_START_ADDRESS;
    int code_length = real_mode_code_end_offset - real_mode_code_start_offset;
    DEBUG("copy real mode code from 0x%p to 0x%p length 0x%p bytes", code_src_address, code_dst_address, code_length);
    memcpy(code_dst_address, code_src_address, code_length);
}

void get_e820_mmap(e820_mmap_t *mmap_output)
{
    call_real_mode_function_wrapper(bios_mmap);
    e820_mmap_t *mmap = (e820_mmap_t *)E820_MMAP_OUTPUT_ADDRESS;
    int length = 2 + 24 * mmap->count;
    DEBUG("mmap found with length of 0x%p bytes", length);
    memcpy(mmap_output, mmap, length);
}

void print_e820_mmap(e820_mmap_t *mmap)
{
    DEBUG("mmap count: %d", mmap->count);
    DEBUG("base             length           type             attributes");
    for (int i = 0; i < mmap->count; i++)
    {
        e820_mmap_entry_t entry = mmap->entries[i];
        DEBUG("%p %p %p %p", entry.base, entry.length, entry.type, entry.attributes);
    }
}