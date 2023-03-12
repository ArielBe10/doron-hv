%ifndef _MACROS
%define _MACROS

%define CODE_START_ADDRESS  0x3300000
%define STACK_START_ADDRESS 0x2800000

%define REAL_MODE_CODE_START_ADDRESS    0x3000
%define REAL_MODE_STACK_START_ADDRESS   0x2000

%define APIC_STACK_START_ADDRESS 0x7000
%define APIC_SEMAPHORE_ADDRESS 0x7100
%define APIC_FUNC_POINTER_ADDRESS 0x7200
%define APIC_GDT_ADDRESS 0x1000

%define PAGING_STRUCTS_ADDRESS  0x1000000
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

%endif
