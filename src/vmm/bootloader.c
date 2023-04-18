#include "vmm/bootloader.h"
#include "vmm/bios.h"
#include "lib/logging.h"

#define BOOTABLE_SIGNATURE 0xaa55
#define MBR_SIZE 512
#define MBR_DESTINATION 0x7c00


int read_mbr(void) {
    bios_dap_t dap;
    dap.size = 16;
    dap.zero = 0;
    dap.sectors_count = 1;
    dap.lba_first_sector = 0;
    dap.dst_segment = 0;
    dap.dst_offset = MBR_DESTINATION;

    for (uint8_t drive_number = 0x80; drive_number < 0xff; drive_number++) {
        DEBUG("reading from drive number: 0x%x", drive_number);
        read_disk(&dap, drive_number);
        if (*(uint16_t*)((uint8_t*)MBR_DESTINATION + MBR_SIZE - sizeof(uint16_t)) == BOOTABLE_SIGNATURE) {
            INFO("MBR is bootable at drive number: 0x%x", drive_number);
            return 0;
        }
    }
    ERROR("bootable MBR not found");
    return 1;
}