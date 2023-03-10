

LINKER = ld 
LINKER_FLAGS = -nostdlib -T linker.ld

ASSEMBLER = nasm
ASSEMBLER_FLAGS = -f elf64

C_COMPILER = gcc
C_COMPILER_FLAGS = -I ./include -nostdlib -c -Werror -Wall -mno-red-zone -ffreestanding -mno-red-zone -g -Wno-packed-bitfield-compat


SRC_DIR = src
OBJDIR = build
ISO_DIR = iso
INCLUDE_DIR = include
LOG_FILE = hypervisor.log

ASM_ENTRY_FILE = $(SRC_DIR)/boot/entrypoint.asm
ASM_FILES = $(wildcard $(SRC_DIR)/**/*.asm)
ASM_OBJ_FILE = $(OBJDIR)/boot/entrypoint.o

C_FILES = $(wildcard $(SRC_DIR)/**/*.c)
HEADER_FILES = $(wildcard $(INCLUDE_DIR)/**/*.h)
C_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(C_FILES))

OBJ_FILES = $(C_OBJ_FILES) $(ASM_OBJ_FILE)


all: $(OBJDIR)/hypervisor.iso


$(ASM_OBJ_FILE): $(ASM_FILES)
	$(ASSEMBLER) $(ASSEMBLER_FLAGS) $(ASM_ENTRY_FILE) -o $@

$(C_OBJ_FILES): $(OBJDIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES)
	$(C_COMPILER) $(C_COMPILER_FLAGS) $< -o $@

$(OBJDIR)/hypervisor.so: $(OBJ_FILES)
	$(LINKER) $(LINKER_FLAGS) $(OBJ_FILES) -o $@

$(OBJDIR)/hypervisor.iso: $(OBJDIR)/hypervisor.so $(ISO_DIR)/boot/grub/grub.cfg
	cp $< $(ISO_DIR)/boot/$(notdir $<)
	grub-mkrescue -o $@ $(ISO_DIR)


.PHONY: clean all

clean:
	rm -f $(OBJ_FILES) $(OBJDIR)/hypervisor.so $(OBJDIR)/hypervisor.iso $(ISO_DIR)/boot/hypervisor.so $(LOG_FILE)