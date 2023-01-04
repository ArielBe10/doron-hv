global real_mode_code_start_offset
global real_mode_code_end_offset
global call_real_mode_function_wrapper
global bios_mmap
global bios_read_disk

[bits 64]
; call real mode function using an absolute call (functions are in lower memory)
call_real_mode_function_wrapper:
    ; first argument is in rdi
    ; change function pointer addr => REAL_MODE_OFFSET(addr)
    add rdi, REAL_MODE_CODE_START_ADDRESS
    sub rdi, real_mode_code_start_offset
    push rdi

    mov rax, .function_address  
    call [rax]  ; near absolute call
    ret

.function_address:
    dq REAL_MODE_OFFSET(call_real_mode_function)


; real mode code that should be copied to lower memory to be executed
real_mode_code_start_offset:
%include "src/bios/switch_modes.asm"
%include "src/bios/mmap.asm"
%include "src/bios/bios_print.asm"
%include "src/bios/read_disk.asm"

real_mode_code_end_offset: