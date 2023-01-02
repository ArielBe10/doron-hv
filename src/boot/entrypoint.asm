%define CODE_START_ADDRESS  0x3300000
%define STACK_START_ADDRESS 0x2800000

%define REAL_MODE_CODE_START_ADDRESS    0x3000
%define REAL_MODE_STACK_START_ADDRESS   0x2000

%define PAGING_STRUCTS_ADDRESS  0x100000
%define EFER_MSR	0xC0000080

%define REAL_MODE_OFFSET(addr) (addr + REAL_MODE_CODE_START_ADDRESS - real_mode_code_start_offset)

%macro SET_SELECTORS 1
    mov ax, %1
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
%endmacro

extern vmm_main

global real_mode_code_start_offset
global real_mode_code_end_offset
global call_real_mode_function_wrapper

section .data
%include "src/boot/gdt.asm"

section .text 
%include "src/boot/paging.asm"


[bits 32]
_start:
    mov esp, STACK_START_ADDRESS

    lgdt [gdt_descriptor]
    jmp CODESEG32:.reset_codeseg32

.reset_codeseg32:
    SET_SELECTORS DATASEG32
    call setup_paging   ; enters long mode with paging and PAE enabled

[bits 64]
long_mode_entry:
    call vmm_main

    push print_char
    call call_real_mode_function_wrapper
    jmp $


; call real mode function using an absolute call (functions are in lower memory)
call_real_mode_function_wrapper:
    ; change function pointer addr => REAL_MODE_OFFSET(addr)
    mov rax, [rsp + 8]
    add rax, REAL_MODE_CODE_START_ADDRESS
    sub rax, real_mode_code_start_offset
    push rax

    mov edi, .function_address  
    call [edi]  ; near absolute call
    ret 8

.function_address:
    dq REAL_MODE_OFFSET(call_real_mode_function)


; real mode code that should be copied to lower memory to be executed
real_mode_code_start_offset:
%include "src/boot/switch_modes.asm"
%include "src/boot/bios.asm"
real_mode_code_end_offset:

; grub multiboot header
section .multiboot
%include "src/boot/multiboot.asm"