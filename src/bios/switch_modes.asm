[bits 32]
real_mode_idt:
	dw 0x3ff		; 256 entries, 4b each = 1K
	dq 0			; Real Mode IVT @ 0x0000

enter_real_mode:
	cli
    jmp CODESEG16:REAL_MODE_OFFSET(.reset_codeseg16)

[bits 16]
.reset_codeseg16:
    SET_SELECTORS DATASEG16
	lidt [REAL_MODE_OFFSET(real_mode_idt)]

    ; disable protected mode (PE) flag of cr0
	mov eax, cr0
	and eax, ~1
	mov cr0, eax
 
	jmp 0:REAL_MODE_OFFSET(.reset_codeseg_real_mode)   ; far jump to set cs

.reset_codeseg_real_mode:
    ; reset segment selectors
	SET_SELECTORS 0
	sti
	ret 2   ; since called from 32 bit code, remove the trailing zeros of the 4 byte address pushed


enter_protected_mode:
	cli
	; enable protected mode (PE) flag of cr0
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp CODESEG32:REAL_MODE_OFFSET(.reset_codeseg32)

[bits 32]
.reset_codeseg32:
	SET_SELECTORS DATASEG32

    ; since called from 16 bit code, add 2 bytes of zero to return address
    xor eax, eax
    pop ax 
    push eax
    ret


[bits 32]
enter_long_mode:
	cli
	; disable paging
    mov eax, cr0
    and eax, ~(1 << 31)
    mov cr0, eax

    ; enable PAE
    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax

    ; set LME (long mode enable)
    mov ecx, EFER_MSR
    rdmsr   ; value is stored in EDX:EAX
    or eax, (1 << 8)
    wrmsr

    ; set cr3 to pml4t
    mov eax, PAGING_STRUCTS_ADDRESS
    mov cr3, eax

    ; enable paging
    mov eax, cr0
    or eax, (1 << 31)
    mov cr0, eax
    
    ; switched to compatibility mode 
    jmp CODESEG64:REAL_MODE_OFFSET(.reset_codeseg64)
    

[bits 64]
.reset_codeseg64:
    SET_SELECTORS DATASEG64

    ; since called from 32 bit code, add 4 bytes of zero to return address
    ; pop into a 32 bit register is not allowed in 64 bit :(
    sub rsp, 4
    pop rax
    shr rax, 32
    push rax
    ret


[bits 32]
switch_back_to_protected_mode:	; from long mode
	cli
	; disable paging
    mov eax, cr0
    and eax, ~(1 << 31)
    mov cr0, eax

    ; far jumps are not allowed in long mode, so use far return instead
    push CODESEG32
    push REAL_MODE_OFFSET(.reset_codeseg32)
    retf    

[bits 32]
.reset_codeseg32:
    ; unset LME (long mode enable)
    mov ecx, EFER_MSR
    rdmsr   ; value is stored in EDX:EAX
    and eax, ~(1 << 8)
    wrmsr

	; disable PAE
    mov eax, cr4
    and eax, ~(1 << 5)
    mov cr4, eax

    SET_SELECTORS DATASEG32
	ret 4   ; since called from 64 bit code, remove the trailing zeros of the 8 byte address pushed


[bits 64]
; this function accepts a 64 bit pointer to a real mode function on the stack, switches to real mode, executes it and switches back
call_real_mode_function:
    mov rdi, [rsp + 8]
    ; push the old rsp to the real mode stack
    mov rax, rsp
	mov rsp, REAL_MODE_STACK_START_ADDRESS
    push rax

    call switch_back_to_protected_mode
[bits 32]
    call enter_real_mode
[bits 16]
    call di
    call enter_protected_mode
[bits 32]
    call enter_long_mode
[bits 64]
    ; restore old rsp
    pop rax
    mov rsp, rax
    ret 8