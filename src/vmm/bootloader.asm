[bits 16]

global __os_entry


__os_entry:
    SET_SELECTORS 0
    mov dl, [DRIVE_NUMBER_ADDRESS]

    jmp 0:0x7c00  ; absolute jump

    cli
    hlt
    jmp $ - 2
