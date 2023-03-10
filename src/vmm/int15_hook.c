#include "vmm/int15_hook.h"
#include "lib/string.h"
#include "vmm/vmexit.h"
#include "hardware/vmx.h"
#include "hardware/vmcs.h"
#include "vmm/bios.h"
#include "lib/logging.h"

#define RFLAGS_CARRY (1 << 0)
#define E820_MAGIC 0x534D4150


void setup_int15_hook(cpu_shared_data_t *shared_data) {
    // save int 15 handler address
    uint32_t *ivt = (uint32_t *)0;
    shared_data->int15_segment = ivt[0x15] >> 16;
    shared_data->int15_offset = ivt[0x15] && 0xffff; 
    // replace with vmcall 
    ivt[0x15] = VMCALL_INT15_GATE;
    uint8_t vmcall_instruction[] = {0x0f, 0x01, 0xc1};
    memcpy((void *)VMCALL_INT15_GATE, vmcall_instruction, sizeof(vmcall_instruction));
}


void fake_int15_handler(cpu_state_t *state) {
    registers_t *regs = &state->registers;
    e820_mmap_t *fake_mmap = state->cpu_data->shared_data->fake_mmap;
    char carry_flag = 0;

    if (regs->rbx >= fake_mmap->count) {
        carry_flag = 1;
    } else {
        regs->rax = E820_MAGIC;
        regs->rcx = sizeof(e820_mmap_entry_t);  // size of buffer returned
        void *output = (void*)(vmread(VMCS_GUEST_ES_BASE) + regs->rdi);
        memcpy(output, &fake_mmap->entries[regs->rbx++], sizeof(e820_mmap_entry_t));
        if (regs->rbx == fake_mmap->count) {
            regs->rbx = 0;
        }
    }

    // emulate iret
    uint16_t ss = vmread(VMCS_GUEST_SS_BASE);
    regs->rip = *(uint16_t *)(ss + regs->rsp);
    uint32_t cs = *(uint16_t *)(ss + regs->rsp + 2);
    uint16_t flags = *(uint16_t *)(ss + regs->rsp + 4);

    if (carry_flag) {
        flags |= RFLAGS_CARRY;
    } else {
        flags &= ~RFLAGS_CARRY;
    }
    vmwrite(VMCS_GUEST_RFLAGS, flags);
    vmwrite(VMCS_GUEST_CS_BASE, cs << 4);
    regs->rsp += 6;   
}


int mark_mmap_memory_unuseable(e820_mmap_t *mmap, size_t start, size_t length) {
    int new_count = mmap->count;

    for (int i = 0; i < mmap->count; i++) {
        if (mmap->entries[i].type == MMAP_AVAILABLE) {
            size_t mmap_start = mmap->entries[i].base, mmap_end = mmap->entries[i].base + mmap->entries[i].length;

            if (mmap_start < start && start + length < mmap_end) {  // unusable region inside mmap region
                if (new_count == MAX_MMAP_ENTRIES) {
                    ERROR("more than %d entries in fake mmap", MAX_MMAP_ENTRIES);
                    return 1;
                }
                // split region to before and after unusable memory
                mmap->entries[i].length = start - mmap_start;  // before

                memcpy(&mmap->entries[new_count], &mmap->entries[i], sizeof(e820_mmap_entry_t));  // after
                mmap->entries[new_count].base = start + length;
                mmap->entries[new_count].length = mmap_end - (start + length);

                new_count++;
            } else if (start < mmap_start && mmap_end < start + length) {  // mmap region inside unusable region
                for (int j = i; i < new_count - 1; j++) {
                    mmap->entries[j] = mmap->entries[j + 1];
                }
                i--;
                new_count--;
            } else if (start < mmap_start && mmap_start < start + length) {  // interlaced
                mmap->entries[i].base = start + length;
            } else if (mmap_start < start && start < mmap_end) {  // interlaced
                mmap->entries[i].length = start - mmap_start;
            }
        }
    }
    mmap->count = new_count;
    return 0;
}