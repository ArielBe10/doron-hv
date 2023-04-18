[bits 64]


extern syscall_handler
extern sysenter_rsp

global __syscall
global __syscall_handler
global __sysenter



__syscall:
    syscall
    ret


__sysenter:
    mov [sysenter_rsp], rsp
    sysenter


__syscall_handler:
    push rcx    ; rip
    push r11    ; rflags

    call syscall_handler

    pop r11
    pop rcx
    jmp rcx
    ; sysret