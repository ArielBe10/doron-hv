; access: P, DPL[0:1], S, E, DC, RW, A
;   P (present)
;   DPL: 0 for kernel, 3 for usermode
;   S (type): 0 for TSS, 1 for code or data
;   E (executable)
;   DC (direction/conforming): for data, 0 for growing backwards, for code, set to 1 to only allow execution when CPL >= DPL
;   RW: set readable for code and writable for data
;   A (accessed): 0
; flags: G, DB, L, reserved
;   G (granularity): set for limit to be in pages 
;   DB (size): 0 for 16 bit, 1 for 32
;   L (long mode): 1 for long mode CODE segment

gdt_start:
    ; null descriptor
    dd 0x00
    dd 0x00
code_segment64:
    .limit_low          dw 0xffff
    .base_low           dw 0x00
    .base_high          db 0x00
    .access             db 10011010b
    .flags_limit_high   db 10101111b
    .base_higher        db 0x00
data_segment64:
    .limit_low          dw 0xffff
    .base_low           dw 0x00
    .base_high          db 0x00
    .access             db 10010010b
    .flags_limit_high   db 11001111b
    .base_higher        db 0x00

code_segment64_usermode:
    .limit_low          dw 0xffff
    .base_low           dw 0x00
    .base_high          db 0x00
    .access             db 11111010b
    .flags_limit_high   db 10101111b
    .base_higher        db 0x00
data_segment64_usermode:
    .limit_low          dw 0xffff
    .base_low           dw 0x00
    .base_high          db 0x00
    .access             db 11110010b
    .flags_limit_high   db 11001111b
    .base_higher        db 0x00

code_segment32:
    .limit_low          dw 0xffff
    .base_low           dw 0x00
    .base_high          db 0x00
    .access             db 10011010b
    .flags_limit_high   db 11001111b
    .base_higher        db 0x00
data_segment32:
    .limit_low          dw 0xffff
    .base_low           dw 0x00
    .base_high          db 0x00
    .access             db 10010010b
    .flags_limit_high   db 11001111b
    .base_higher        db 0x00

code_segment16:
    .limit_low          dw 0xffff
    .base_low           dw 0x00
    .base_high          db 0x00
    .access             db 10011010b
    .flags_limit_high   db 00000000b
    .base_higher        db 0x00
data_segment16:
    .limit_low          dw 0xffff
    .base_low           dw 0x00
    .base_high          db 0x00
    .access             db 10010010b
    .flags_limit_high   db 00000000b
    .base_higher        db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq gdt_start


%define CODESEG64 code_segment64 - gdt_start
%define DATASEG64 data_segment64 - gdt_start
%define CODESEG64_USERMODE code_segment64_usermode - gdt_start
%define DATASEG64_USERMODE data_segment64_usermode - gdt_start
%define CODESEG32 code_segment32 - gdt_start
%define DATASEG32 data_segment32 - gdt_start
%define CODESEG16 code_segment16 - gdt_start
%define DATASEG16 data_segment16 - gdt_start