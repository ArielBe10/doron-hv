section .text
[bits 64]


global acquire_lock
global release_lock


acquire_lock:
    mov ax, 1
    lock xchg [rdi], ax
    cmp ax, 1
    je acquire_lock
    ret


release_lock:
    mov byte [rdi], 0
    ret