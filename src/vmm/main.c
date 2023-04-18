#include "lib/logging.h"
#include "lib/string.h"
#include "vmm/bios.h"
#include "vmm/kheap.h"
#include "vmm/vmm.h"
#include "vmm/state.h"
#include "vmm/int15_hook.h"
#include "vmm/bootloader.h"
#include "demo/malware.h"
#include "hardware/vmx.h"
#include "demo/syscall.h"


#define MMAP_BUFFER_SIZE 0x100
#define HEAP_MAX_SIZE 0x10000


extern void __vmcall_init(void);


void exit(void)
{
    asm("cli; hlt;");
}


static void print_hello(void) {
    INFO("hello!");
}


void vmm_main()
{
    init_logging(DEBUG_LEVEL);
    INFO("starting vmm_main");

    // mmap size is unknown
    uint8_t e820_mmap_buffer[MMAP_BUFFER_SIZE];
    e820_mmap_t *e820_mmap = (e820_mmap_t *)e820_mmap_buffer;
    get_e820_mmap(e820_mmap);
    print_e820_mmap(e820_mmap);

    // ASSERT(read_mbr() == 0);

    kheap_metadata_t kheap = setup_kheap(e820_mmap, HEAP_MAX_SIZE);
    cpu_shared_data_t *shared_data = create_cpu_data(&kheap);
    set_cpu_data(shared_data, kheap);

    init_vmx_all_cpus(&kheap, shared_data->cpu_states[0]);
    setup_int15_hook(shared_data);

    enter_vmx();
}


void vm_guest_main(void) {
    INFO("VM GUEST MAIN");

    e820_mmap_t mmap;
    get_e820_mmap(&mmap);
    DEBUG("fake mmap:");
    print_e820_mmap(&mmap);

    init_ssdt();
    setup_syscall_handler(0x123, print_hello);

    __vmcall_init();  // antivirus
    malware();  // virus

    __sysenter(0x123);
    __syscall(0x123);

    INFO("finished vmm_main");
    exit();
}