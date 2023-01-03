%ifndef _MACROS
%define _MACROS

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

%endif
