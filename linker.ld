SECTIONS
{
    /* match the CODE_START_ADDRESS from src/boot/multiboot.asm */
    . = 0x3300000;
    .multiboot : { *(.multiboot) }
    .text : { *(.text) }
    .data : { *(.data) } 
    .bss : { *(.bss) }
    .rodata : { *(.rodata) }
}