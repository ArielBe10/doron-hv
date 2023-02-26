#include "lib/string.h"
#include "hardware/rsdp.h"

#include <stddef.h>

#define EBDA_POINTER_ADDRESS 0x40E

const char *RDSP_SIGNATURE = "RSD PTR ";


rsdp_descriptor_v2_t *find_rsdp(void) {
    rsdp_descriptor_v2_t *rsdp;
    size_t ebda_address = (*(uint16_t*)EBDA_POINTER_ADDRESS) >> 4;

    for (int offset = 0; offset < 1024; offset++) {
        rsdp = (rsdp_descriptor_v2_t*)(ebda_address + offset);
        if (memcmp(RDSP_SIGNATURE, &rsdp->first_part.signature, 8, 0) == 0) {
            return rsdp;
        }
    }
    for (size_t address = 0xe0000; address < 0xfffff; address += 16) {
        rsdp = (rsdp_descriptor_v2_t*)(address);
        if (memcmp(RDSP_SIGNATURE, &rsdp->first_part.signature, 8, 0) == 0) {
            return rsdp;
        }
    }
    return NULL;
}


acpi_sdt_header_t *find_sdt_table(rsdp_descriptor_v2_t *rsdp, char *table_signature) {
    rsdt_t *rsdt = (rsdt_t*)(size_t)rsdp->first_part.rsdt_address;

    int entries = (rsdt->header.length - sizeof(rsdt->header)) / 4;
    for (int i = 0; i < entries; i++)
    {
        acpi_sdt_header_t *header = (acpi_sdt_header_t *)(size_t)rsdt->other_sdt_pointers[i];
        if (memcmp(header->signature, table_signature, 4, 0) == 0)
            return header;
    }
    return NULL;
}


int get_cpu_count(madt_t *madt) {
    int count = 0;

    size_t entry_address = (size_t)&madt->entry0;
    while (entry_address < (size_t)madt + madt->header.length) {
        madt_entry_header_t *entry = (madt_entry_header_t*)entry_address;
        if (entry->entry_type == 0) {
            count++;
        }
        entry_address += entry->entry_length;
    }
    return count;
}


void get_cpu_ids(madt_t *madt, uint8_t *cpu_ids) {
    int cpu_index = 0;

    size_t entry_address = (size_t)&madt->entry0;
    while (entry_address < (size_t)madt + madt->header.length) {
        madt_entry_header_t *entry = (madt_entry_header_t*)entry_address;
        if (entry->entry_type == 0) {
            cpu_ids[cpu_index++] = ((madt_entry0_t*)entry)->cpu_id;
        }
        entry_address += entry->entry_length;
    }
}