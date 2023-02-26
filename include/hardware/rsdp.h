#pragma once

#include <stdint.h>

typedef struct {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed)) rsdp_descriptor_v1_t;


typedef struct {
    rsdp_descriptor_v1_t first_part;

    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed)) rsdp_descriptor_v2_t;


typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed)) acpi_sdt_header_t;


typedef struct {
    acpi_sdt_header_t header;
    uint32_t other_sdt_pointers[];
} __attribute__((packed)) rsdt_t;


typedef struct {
    uint8_t entry_type;
    uint8_t entry_length;
} __attribute__((packed)) madt_entry_header_t;


typedef struct {
    madt_entry_header_t header;
    uint8_t cpu_id;
    uint8_t apic_id;
    uint32_t flags;
} __attribute__((packed)) madt_entry0_t;


typedef struct {
    acpi_sdt_header_t header;
    uint32_t local_apic_address;
    uint32_t flags;
    madt_entry_header_t entry0;
} __attribute__((packed)) madt_t;


rsdp_descriptor_v2_t *find_rsdp(void);
acpi_sdt_header_t *find_sdt_table(rsdp_descriptor_v2_t *rsdp, char *table_signature);
int get_cpu_count(madt_t *madt);
void get_cpu_ids(madt_t *madt, uint8_t *cpu_ids);
