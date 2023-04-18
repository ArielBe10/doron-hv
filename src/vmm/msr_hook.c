#include "vmm/msr_hook.h"
#include "lib/logging.h"


typedef enum {
    MSR_RANGE_FIRST = 0,
    MSR_RANGE_SECOND = 1
} msr_range_t;


static bool is_msr_valid(uint64_t msr_number, msr_range_t *range)
{
    if (msr_number >= 0 && msr_number <= 0x1fff) {
        *range = MSR_RANGE_FIRST;
        return 1;
    }
    else if (msr_number >= 0xc0000000 && msr_number <= 0xc0001fff) {
        *range = MSR_RANGE_SECOND;
        return 1;
    }
    return 0;
}


int update_msr_access_policy(cpu_shared_data_t *shared_data, uint64_t msr_number, bool exit_read, bool exit_write)
{
    msr_range_t range;
    uint64_t msr_read_idx, msr_write_idx;
    uint8_t *bitmap;

    if (!is_msr_valid(msr_number, &range)) {
        ERROR("invalid msr: %p", msr_number);
        return 1;
    }
    
    msr_read_idx = (range == MSR_RANGE_FIRST) ? msr_number / 8 : (msr_number - 0xc0000000) / 8 + 1024;
    msr_write_idx = (range == MSR_RANGE_FIRST) ? msr_number / 8 + 2048 : (msr_number - 0xc0000000) / 8 + 3072;
    bitmap = shared_data->msr_bitmap;

    if (exit_read) {
        bitmap[msr_read_idx] |= (1 << (msr_number % 8));
    }
    else {
        bitmap[msr_read_idx] &= ~(1 << (msr_number % 8));
    }
    if (exit_write) {
        bitmap[msr_write_idx] |= (1 << (msr_number % 8));
    }
    else {
        bitmap[msr_write_idx] &= ~(1 << (msr_number % 8));
    }
    return 0;
}