#include "vmm/paging.h"
#include "lib/logging.h"


void create_paging_tables(paging_tables_t *paging_tables) {
    paging_tables->pml4_tables[0] = (size_t)&paging_tables->pdp_tables[0] | PAGING_RW | PAGING_PRESENT;

    // initialize pdp tables
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        paging_tables->pdp_tables[i] = (size_t)&paging_tables->pd_tables[i * PAGE_TABLE_LENGTH] | PAGING_RW | PAGING_PRESENT;
    }

    // initialize pd tables
    size_t physical_address = 0;
    for (size_t i = 0; i < PAGE_TABLE_LENGTH * MEMORY_SIZE; i++) {
        // for this field, memory type is stored in bits 3:5
        paging_tables->pd_tables[i] = physical_address | PAGING_LARGE_PAGE | PAGING_RW | PAGING_PRESENT;
        physical_address += LARGE_PAGE_SIZE;
    }
}


void create_ept_paging_tables(ept_paging_tables_t *ept_paging_tables) {
    ept_paging_tables->eptp = EPT_MEMORY_TYPE_WRITEBACK | EPT_PAGE_WALK_LENGTH | 
        (size_t)&ept_paging_tables->ept_pml4_tables[0];
    ept_paging_tables->ept_pml4_tables[0] = EPT_RWX | (size_t)&ept_paging_tables->ept_pdp_tables[0];

    // initialize ept pdp tables
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        ept_paging_tables->ept_pdp_tables[i] = EPT_RWX | (size_t)&ept_paging_tables->ept_pd_tables[i * PAGE_TABLE_LENGTH];
    }
    // initialize ept pd tables
    for (size_t i = 0; i < PAGE_TABLE_LENGTH * MEMORY_SIZE; i++) {
        ept_paging_tables->ept_pd_tables[i] = EPT_RWX | (size_t)&ept_paging_tables->ept_page_tables[i * PAGE_TABLE_LENGTH];
    }
    // initialize ept page tables
    size_t physical_address = 0;
    for (size_t i = 0; i < PAGE_TABLE_LENGTH * PAGE_TABLE_LENGTH * MEMORY_SIZE; i++) {
        // for this field, memory type is stored in bits 3:5
        ept_paging_tables->ept_page_tables[i] = (EPT_MEMORY_TYPE_WRITEBACK << 3) | EPT_RWX | physical_address;
        physical_address += PAGE_SIZE;
    }
}

void update_ept_access_rights(ept_paging_tables_t *ept_paging_tables, size_t start, size_t length, uint8_t access_rights) {
    ASSERT(start % PAGE_SIZE == 0);
    ASSERT(length % PAGE_SIZE == 0);
    ASSERT(access_rights <= 7);
    
    size_t page_index = start / PAGE_SIZE;
    for (size_t i = 0; i < length / PAGE_SIZE; i++) {
        ept_paging_tables->ept_page_tables[page_index] &= ~7ULL;  // remove all rights
        ept_paging_tables->ept_page_tables[page_index] |= access_rights;  // grant desired rights
        page_index++;
    }
}