#include "lib/logging.h"
#include "lib/string.h"
#include "vmm/bios.h"
#include "vmm/kheap.h"
#include "vmm/vmm.h"
#include "vmm/state.h"

void exit(void)
{
    asm("cli; hlt;");
}

void vmm_main()
{
    init_logging(DEBUG_LEVEL);
    INFO("starting vmm_main");

    init_bios();

    // mmap size is unknown
    int e820_mmap_buffer[0x100];
    e820_mmap_t *e820_mmap = (e820_mmap_t *)e820_mmap_buffer;
    get_e820_mmap(e820_mmap);
    print_e820_mmap(e820_mmap);

    bios_dap_t dap;
    dap.size = 16;
    dap.zero = 0;
    dap.sectors_count = 1;
    dap.lba_first_sector = 0;
    dap.dst_segment = 0;
    dap.dst_offset = 0x1300;
    read_disk(&dap);

    kheap_metadata_t kheap = setup_kheap(e820_mmap, 0x10000);
    shared_cpu_state_t *shared_cpu_state = create_cpu_states(&kheap);
    INFO("shared cpu state created: %p", shared_cpu_state);

    // enter_vmx(&kheap);

    INFO("finished vmm_main");
    exit();
}
