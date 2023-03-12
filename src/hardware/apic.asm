[bits 16]
section .text

global apic_start


apic_start:
    mov sp, APIC_STACK_START_ADDRESS

    lgdt [REAL_MODE_OFFSET(apic_gdt_descriptor)]
    jmp 0:REAL_MODE_OFFSET(.reset_codeseg16)

.reset_codeseg16:
    SET_SELECTORS 0

    call enter_protected_mode
[bits 32]
    call enter_long_mode
[bits 64]
    call qword [APIC_FUNC_POINTER_ADDRESS]

    mov byte [APIC_SEMAPHORE_ADDRESS], 1

    ; sleep
    cli
    hlt
    jmp $ - 2


apic_gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq APIC_GDT_ADDRESS
