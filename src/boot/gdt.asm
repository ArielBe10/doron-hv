gdt_start:
    ; null descriptor
    dd 0x00
    dd 0x00

code_segment:
    dw 0xffff ; limit 15:0
    dw 0x00 ; base 15:0
    
    db 0x00 ; base 23:16
    db 10011010b ; P, DPL, S, type
    db 10101111b ; G, D/B, L, AVL, limit 19:16
    db 0x00 ; base 31:24

data_segment:
    dw 0xffff ; limit 15:0
    dw 0x00 ; base 15:0

    db 0x00 ; base 23:16
    db 10010010b ; P, DPL, S, type
    db 11001111b ; G, D/B, L, AVL, limit 19:16
    db 0x00 ; base 31:24

gdt_end:

%define CODESEG code_segment - gdt_start
%define DATASEG data_segment - gdt_start

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start