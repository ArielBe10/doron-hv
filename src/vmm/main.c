#include "lib/logging.h"
#include "vmm/init_bios.h"

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
    int buffer[0x100];
    e820_mmap_t *e820_mmap = (e820_mmap_t *)buffer;
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
    char *disk = (char *)0x1300;
    disk[10] = '\0';
    INFO("%s", disk);

    INFO("finished vmm_main");
    exit();
}
