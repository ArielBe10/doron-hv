SECTIONS
{
    /* match the CODE_BEGIN_ADDRESS from entrypoint.asm */
    . = 0x33000;
    .text : { *(.text) }
    .data : { *(.data) } 
    .bss : { *(.bss) }
    .rodata : { *(.rodata) }
}