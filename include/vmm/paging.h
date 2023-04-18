#pragma once

#include <stdint.h>
#include <stddef.h>


#define MEMORY_SIZE 4  // in GB
#define PAGE_SIZE 0x1000
#define LARGE_PAGE_SIZE 0x200000
#define PAGE_TABLE_LENGTH 512


typedef enum {
    EPT_MEMORY_TYPE_UNCACHEABLE = 0,
    EPT_MEMORY_TYPE_WRITEBACK = 6
} ept_memory_type_t;


#define EPT_PAGE_WALK_LENGTH (3 << 3)
#define EPT_READ 1
#define EPT_WRITE (1 << 1)
#define EPT_EXECUTE (1 << 2)
#define EPT_RWX (EPT_READ | EPT_WRITE | EPT_EXECUTE)
#define EPT_RW (EPT_READ | EPT_WRITE)
#define EPT_WX (EPT_WRITE | EPT_EXECUTE)
#define EPT_RX (EPT_READ | EPT_EXECUTE)

#define PAGING_PRESENT 1
#define PAGING_RW (1 << 1)
#define PAGING_LARGE_PAGE (1 << 7)


typedef struct {
    // large 2 MB pages for 512 GB memory size (1 pml4 entry)
    size_t pml4_tables[PAGE_TABLE_LENGTH];  // 1 pml4 table with 1 entry (512 GB each entry)
    size_t pdp_tables[PAGE_TABLE_LENGTH];  // 1 pdp table with MEMORY_SIZE entries (1 GB each entry)
    size_t pd_tables[PAGE_TABLE_LENGTH * MEMORY_SIZE];  // 1 pd table with 512 entries for every pdp table entry (2 MB each entry)
} paging_tables_t;

typedef struct {
    // normal 2 KB pages for MEMORY_SIZE GB of memory
    size_t eptp;
    size_t pad[PAGE_TABLE_LENGTH - 1];
    size_t ept_pml4_tables[PAGE_TABLE_LENGTH];  // 1 ept pml4 table with 1 entry (512 GB each entry)
    size_t ept_pdp_tables[PAGE_TABLE_LENGTH];  // 1 ept pdp table with MEMORY_SIZE entries (1 GB each entry)
    size_t ept_pd_tables[PAGE_TABLE_LENGTH * MEMORY_SIZE];  // 1 ept pd table with 512 entries for every ept pdp table entry (2 MB each entry)
    size_t ept_page_tables[PAGE_TABLE_LENGTH * PAGE_TABLE_LENGTH * MEMORY_SIZE];  // 1 ept page table for every ept pd table entry (2 KB each entry)
} ept_paging_tables_t;


void create_ept_paging_tables(ept_paging_tables_t *ept_paging_tables);
void create_paging_tables(paging_tables_t *paging_tables);
void update_ept_access_rights(ept_paging_tables_t *ept_paging_tables, size_t start, size_t length, uint8_t access_rights);
