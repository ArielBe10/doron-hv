%define DAP_ADDRESS 0x1200
%define DRIVE_NUMBER_ADDRESS 0x6000

[bits 16]
bios_read_disk:
	mov si, DAP_ADDRESS	; address of "disk address packet"
	mov ah, 0x42		; read disk
	mov dl, [DRIVE_NUMBER_ADDRESS]
	int 0x13
	jc .error

    push REAL_MODE_OFFSET(read_disk_success_msg)
    call bios_println
    ret 

.error:
    push REAL_MODE_OFFSET(read_disk_error_msg)
    call bios_println

    push REAL_MODE_OFFSET(read_disk_error_code_msg)
    call bios_print

    ; error code in ah
    mov al, ah
    xor ah, ah
    push ax
    call bios_print_hex

    jmp $

read_disk_error_msg db "error reading disk :(", 0
read_disk_error_code_msg db "error code: 0x", 0
read_disk_success_msg db "disk read successfully", 0