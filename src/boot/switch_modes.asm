%define REAL_MODE_STACK_START_ADDRESS 0x2000

[bits 32]

real_mode_idt:
	dw 0x3ff		; 256 entries, 4b each = 1K
	dq 0			; Real Mode IVT @ 0x0000

enter_real_mode:
	cli
    jmp CODESEG16:.reset_codeseg16

[bits 16]

.reset_codeseg16:
    mov eax, DATASEG16
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax

	lidt [real_mode_idt]

    ; disable paging and protected mode
	mov eax, cr0
	and eax, ~1
	mov cr0, eax
 
	jmp 0:.reset_codeseg_real_mode   ; far jump to set cs

.reset_codeseg_real_mode:
    ; reset segment selectors
	mov ax, 0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov sp, REAL_MODE_STACK_START_ADDRESS

	sti

call_interrupt:
	mov ax, 0
	int 0x10

    mov ah, 0x0e
    mov al, 'a'
    int 0x10

    jmp $
