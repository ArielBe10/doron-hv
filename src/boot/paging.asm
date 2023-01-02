%define PAGING_PDPT_ENTRY_COUNT 16
%define PAGING_DEFAULT_FLAGS    0x3  ; rwx present
%define PAGING_LARGE_PAGE_FLAG  (1 << 7)
%define LARGE_PAGE_SIZE         0x200000


[bits 32]
; Create a paging table
; params:
;   eax: where the table entries should point to: eax, eax+0x1000, ...
;   ebx: paging table address
;   ecx: how many table entries to create
;   edx: paging flags for table entries
;   edi: page size
create_paging_tables:
.setup_pdpts:
    mov dword [ebx+4], 0  ; paging entry is 8 bytes
    mov [ebx], eax
    or [ebx], edx     ; set entry flags
    add ebx, 8          ; proceed to next entry
    add eax, edi        ; point to next page
    loop .setup_pdpts   ; repeat ecx times
    ret


setup_paging:
    ; create pml4t entry
    mov ebx, PAGING_STRUCTS_ADDRESS
    mov eax, ebx
    add eax, 0x1000
    mov ecx, 1      ; create a single entry
    mov edx, PAGING_DEFAULT_FLAGS
    mov edi, 0x1000 
    call create_paging_tables

    ; create pdpt entries
    mov ebx, PAGING_STRUCTS_ADDRESS
    add ebx, 0x1000
    mov eax, ebx
    add eax, 0x1000
    mov ecx, PAGING_PDPT_ENTRY_COUNT
    push eax    ; where to create the pdts
    call create_paging_tables

    ; create pdt entries
    pop ebx                                 ; create the pdts where the pdpts are pointing to 
    xor eax, eax                            ; start at address 0
    mov ecx, 512 * PAGING_PDPT_ENTRY_COUNT  ; create 512 entries for every pdpt entry
    or edx, PAGING_LARGE_PAGE_FLAG          ; create large pages 
    mov edi, LARGE_PAGE_SIZE
    call create_paging_tables

    ret