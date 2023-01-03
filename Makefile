

LINKER = ld 
LINKER_FLAGS = -nostdlib -T linker.ld

ASSEMBLER = nasm
ASSEMBLER_FLAGS = -f elf64

C_COMPILER = gcc
C_COMPILER_FLAGS = -I ./include -nostdlib -c -Werror -Wall -mno-red-zone -ffreestanding -mno-red-zone -g


SRC_DIR = src
OBJDIR = build
ISO_DIR = iso
INLCUDE_DIR = include
LOG_FILE = hypervisor.log

ASM_FILES = $(wildcard $(SRC_DIR)/**/*.asm)
C_FILES = $(wildcard $(SRC_DIR)/**/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(C_FILES)) \
			$(OBJDIR)/boot/entrypoint.o


all: $(OBJDIR)/hypervisor.iso


$(OBJDIR)/boot/entrypoint.o: $(ASM_FILES)
	$(ASSEMBLER) $(ASSEMBLER_FLAGS) $(SRC_DIR)/boot/entrypoint.asm -o $@

$(OBJDIR)/%.o : $(SRC_DIR)/%.c
	$(C_COMPILER) $(C_COMPILER_FLAGS) $< -o $@

$(OBJDIR)/hypervisor.so: $(OBJ_FILES)
	$(LINKER) $(LINKER_FLAGS) $(OBJ_FILES) -o $@

$(OBJDIR)/hypervisor.iso: $(OBJDIR)/hypervisor.so $(ISO_DIR)/boot/grub/grub.cfg
	cp $< $(ISO_DIR)/boot/$(notdir $<)
	grub-mkrescue -o $@ $(ISO_DIR)


.PHONY: clean all

clean:
	rm -f $(OBJ_FILES) $(OBJDIR)/hypervisor.so $(OBJDIR)/hypervisor.iso $(ISO_DIR)/boot/hypervisor.so $(LOG_FILE)