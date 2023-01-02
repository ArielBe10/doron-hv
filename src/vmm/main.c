#include "lib/logging.h"
#include "lib/string.h"

#define REAL_MODE_CODE_START_ADDRESS 0x3000

extern void real_mode_code_start_offset(void);
extern void real_mode_code_end_offset(void);

void copy_real_mode_code();

void copy_real_mode_code()
{
    char *code_src_address = (char *)real_mode_code_start_offset;
    char *code_dst_address = (char *)REAL_MODE_CODE_START_ADDRESS;
    int code_length = real_mode_code_end_offset - real_mode_code_start_offset;

    DEBUG("copying real mode code: src=0x%p, dst=0x%p, length=0x%p", code_src_address, code_dst_address, code_length);
    memcpy(code_dst_address, code_src_address, code_length);
}

void vmm_main()
{
    init_logging(DEBUG_LEVEL);
    INFO("hello world");
    copy_real_mode_code();
}
