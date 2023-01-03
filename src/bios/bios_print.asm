[bits 16]

; print a null terminated ascii message
; params:
;   dw message: the address of the message to be printed
bios_print:
    push bp
    mov bp, sp
    pusha

    mov bx, [bp + 4]
    mov ah, 0x0e
.loop:
    mov al, [bx]
    cmp al, 0
    je .end
    int 0x10
    inc bx
    jmp .loop
.end:

    popa
    pop bp
    ret 2

; print a null terminated ascii message and a newline (\r\n)
; params:
;   dw message: the address of the message to be printed
bios_println:
    push bp
    mov bp, sp
    
    push word [bp + 4]
    call bios_print
    push REAL_MODE_OFFSET(newline)
    call bios_print

    pop bp
    ret 2

; print a hexadecimal number
; params:
;   dw number: the number to print
bios_print_hex:
    push bp
    mov bp, sp
    pusha

    mov ax, [bp + 4]
    mov bx, REAL_MODE_OFFSET(.hex_msg)
    mov di, 3

.loop:
    mov dx, ax
    and dx, 0x000f
    cmp dx, 0xa
    jb .small_hex
    add dx, 'a'
    sub dx, 0xa
    jmp .set

.small_hex:
    add dx, '0'

.set:
    mov [bx + di], dl

    shr ax, 4
    dec di
    cmp di, 0
    jne .loop

    push REAL_MODE_OFFSET(.hex_msg)
    call bios_print

    popa
    pop bp
    ret 2


.hex_msg db "0000", 0
newline db 0x0d, 0x0a, 0   ; "\r\n"
space db ' ', 0