SECTIONS
{
    /* match the CODE_START_ADDRESS from src/boot/macros.asm */
    . = 0x3300000;
    .multiboot : { *(.multiboot) }
    .text : { *(.text) }
    .data : { *(.data) } 
    .bss : { *(.bss) }
    .rodata : { *(.rodata) }
}