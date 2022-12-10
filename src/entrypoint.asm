%define MULTIBOOT_MAGIC 0xE85250D6
%define CODE_BEGIN_ADDRESS 0x33000

[bits 32]
segment .text

multiboot2_header_start:
    .magic          dd  MULTIBOOT_MAGIC
    .architecture   dd  0   ; i386
    .header_length  dd  multiboot2_header_end - multiboot2_header_start
    .checksum       dd  0x100000000 - (MULTIBOOT_MAGIC + (multiboot2_header_end - multiboot2_header_start) + 0)
    
    multiboot2_address_tag_start:
        .type           dw  2
        .flags          dw  0
        .size           dd  multiboot2_address_tag_end - multiboot2_address_tag_start
        .header_addr    dd  CODE_BEGIN_ADDRESS  ; load multiboot2 header at this physical address
        .load_addr      dd  -1                  ; start loading from this image offset (-1 means load from the start of the image)
        .load_end_addr  dd  0                   ; end loading at this image offset (0 means load until the end of the image)
        .bss_end_addr   dd  0                   ; physical address of the end of the bss (0 means don't allocate a bss)
    multiboot2_address_tag_end:

    multiboot2_entry_address_tag_start:
        .type       dw  3
        .flags      dw  0
        .size       dd  multiboot2_entry_address_tag_end - multiboot2_entry_address_tag_start
        .entrypoint dd  _start
    multiboot2_entry_address_tag_end:
    
        dd 0
        dd 0
        dw 8
multiboot2_header_end:


_start:
    jmp short $ 