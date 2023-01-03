%define MMAP_OUTPUT_ADDRESS 0x1000

[bits 16]
; use the int 0x15 with eax = 0xE820 BIOS function to get a memory map
; output structure (stored in MMAP_OUTPUT_ADDRESS):
;	dw entry_count: how many entries were read
;	24 bytes entry_array: an array of size entry_count of the memory entries read

; for additional info on the int 0x15 eax=0xE820 interrupt: http://www.uruk.org/orig-grub/mem64mb.html
bios_mmap:
	mov eax, 0x0
	mov es, eax
	mov di, MMAP_OUTPUT_ADDRESS
	add di, 2			; add 2 for entry_count size
	xor si, si			; entry count = 0

	; first call
	xor ebx, ebx				; must be 0 for the first call
	mov edx, 0x0534D4150		; must be "SMAP"
	mov eax, 0xe820				; function code
	mov ecx, 24					; ask for 24 byte entries
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	int 0x15

	jc .failed				; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; restore value
	cmp eax, edx			; on success, eax must have been reset to "SMAP"
	jne .failed
	test ebx, ebx			; should be passed to next BIOS call
							; set to 0 for last descriptior, ebx = 0 implies list is only 1 entry long (worthless)
	je .failed

	; print header
	push REAL_MODE_OFFSET(mmap_header_msg)
	call bios_println
	push REAL_MODE_OFFSET(mmap_fields_msg)
	call bios_println

	jmp .check_entry

.loop:
	; restore trashed registers
	mov eax, 0xe820
	mov ecx, 24
	mov [es:di + 20], dword 1
	mov edx, 0x0534D4150	; repair potentially trashed register
	int 0x15

	jc .finished		; carry set means "end of list already reached"

.check_entry:
	jcxz .skip_entry		; jmp if ecx = 0, ecx is the number of bytes returned (skip any 0 length entries)
	cmp cl, 20				; got a 24 byte ACPI 3.X response?
	jbe .skip_test
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je .skip_entry

.skip_test:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skip_entry			; if length uint64_t is 0, skip entry

	call print_mmap_entry
	inc si					; good entry: increment counter and set di to the next entry address
	add di, 24


.skip_entry:
	test ebx, ebx			; if ebx resets to 0, list is complete
	jne .loop

.finished:
    mov di, MMAP_OUTPUT_ADDRESS		; restore to original pointer to output
	mov word [di], si	; store the entry count
	clc					; clear carry flag from last interrupt

	; print count
	push REAL_MODE_OFFSET(mmap_count_msg)
	call bios_print
	push si
	call bios_print_hex
	push REAL_MODE_OFFSET(newline)
	call bios_print

	push REAL_MODE_OFFSET(mmap_success_msg)
	call bios_println
	ret

.failed:
	push REAL_MODE_OFFSET(mmap_error_msg)
	call bios_print
	jmp short $


; print a 24 byte mmap entry
; params:
;	es:di address of entry
print_mmap_entry:
	; address
	push word [es:di + 6]
	call bios_print_hex
	push word [es:di + 4]
	call bios_print_hex
	push word [es:di + 2]
	call bios_print_hex
	push word [es:di + 0]
	call bios_print_hex
	push REAL_MODE_OFFSET(space)
	call bios_print

	; length
	push word [es:di + 14]
	call bios_print_hex
	push word [es:di + 12]
	call bios_print_hex
	push word [es:di + 10]
	call bios_print_hex
	push word [es:di + 8]
	call bios_print_hex
	push REAL_MODE_OFFSET(space)
	call bios_print

	; type
	push word [es:di + 18]
	call bios_print_hex
	push word [es:di + 16]
	call bios_print_hex
	push REAL_MODE_OFFSET(space)
	call bios_print

	; attributes
	push word [es:di + 22]
	call bios_print_hex
	push word [es:di + 20]
	call bios_print_hex
	push REAL_MODE_OFFSET(space)
	call bios_print

	push REAL_MODE_OFFSET(newline)
	call bios_print

	ret

mmap_error_msg db "error reading mmap :(", 0
mmap_success_msg db "mmap read successfully", 0
mmap_new_region_msg db "new region: ", 0
mmap_header_msg db "reading mmap:", 0
mmap_fields_msg db "base_address     length           type     attributes", 0
mmap_count_msg db "mmap entry count: ", 0