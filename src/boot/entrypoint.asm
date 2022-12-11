%define STACK_START_ADDRESS 0x2800000
%define PAGING_STRUCTS_ADDRESS 0x100000
%define PAGING_PDPT_ENTRY_COUNT 16
%define PAGING_DEFAULT_FLAGS 0x3    ; rwx present
%define PAGING_LARGE_PAGE_FLAG (1 << 7)
%define LARGE_PAGE_SIZE 0x200000
%define EFER_MSR 0xC0000080

extern vmm_main

segment .data
%include "src/boot/gdt.asm"

[bits 32]
segment .text 

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

_start:
    mov esp, STACK_START_ADDRESS

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

    ; disable paging
    mov eax, cr0
    and eax, ~(1 << 31)
    mov cr0, eax

    ; enable PAE
    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax

    ; set LME (long mode enable)
    mov ecx, EFER_MSR
    rdmsr   ; value is stored in EDX:EAX
    or eax, (1 << 8)
    wrmsr

    ; set cr3 to pml4t
    mov eax, PAGING_STRUCTS_ADDRESS
    mov cr3, eax

    ; enable paging
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax
    
    ; switched to compatibility mode 
    lgdt [gdt_descriptor]
    jmp CODESEG:long_mode_entry
    

[bits 64]
long_mode_entry:
    cli
    mov ax, DATASEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call vmm_main


segment .multiboot
%include "src/boot/multiboot.asm"