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

    INFO("finished vmm_main");
    exit();
}
