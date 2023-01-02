[bits 16]
print_char:
	mov ax, 0
	int 0x10

    mov ah, 0x0e
    mov al, 'a'
    int 0x10

    ret