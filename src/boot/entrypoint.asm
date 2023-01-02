%define CODE_START_ADDRESS  0x3300
%define STACK_START_ADDRESS 0x2800000

%define REAL_MODE_STACK_START_ADDRESS       0x2000

%define PAGING_STRUCTS_ADDRESS  0x100000

%macro SET_SELECTORS 1
    mov ax, %1
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
%endmacro

extern vmm_main

section .data
%include "src/boot/gdt.asm"

section .text 
%include "src/boot/paging.asm"
%include "src/boot/switch_modes.asm"


[bits 32]
_start:
    mov esp, STACK_START_ADDRESS

    lgdt [gdt_descriptor]
    jmp CODESEG32:.reset_codeseg32

.reset_codeseg32:
    SET_SELECTORS DATASEG32
    call setup_paging
    call enter_long_mode

[bits 64]
long_mode_entry:
    push print_char
    call call_real_mode_function

    call vmm_main

    jmp $


[bits 16]
print_char:
	mov ax, 0
	int 0x10

    mov ah, 0x0e
    mov al, 'a'
    int 0x10

    ret

section .multiboot
%include "src/boot/multiboot.asm"