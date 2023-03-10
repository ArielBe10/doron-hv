%include "src/boot/macros.asm"

extern vmm_main
global __hypervisor_start
global __hypervisor_end

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

; include real mode functions
%include "src/bios/real_mode.asm"
%include "src/vmm/vmm.asm"

; grub multiboot header
section .multiboot
__hypervisor_start:

%include "src/boot/multiboot.asm"

section .end
__hypervisor_end: