SECTIONS
{
    /* match the CODE_BEGIN_ADDRESS from src/boot/multiboot.asm */
    . = 0x330000;
    .multiboot : { *(.multiboot) }
    .text : { *(.text) }
    .data : { *(.data) } 
    .bss : { *(.bss) }
    .rodata : { *(.rodata) }
}