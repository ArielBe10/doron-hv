

LINKER = ld 
LINKER_FLAGS = -nostdlib -T linker.ld

ASSEMBLER = nasm
ASSEMBLER_FLAGS = -f elf64

C_COMPILER = gcc
C_COMPILER_FLAGS = -nostdlib -c 


SRC_DIR = src
OBJDIR = build
ISO_DIR = iso
INLCUDE_DIR = include
LOG_FILE = hypervisor.log

ASM_FILES = $(wildcard $(SRC_DIR)/**/*.asm) $(wildcard $(SRC_DIR)/*.asm)
C_FILES = $(wildcard $(SRC_DIR)/**/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(C_FILES)) $(patsubst $(SRC_DIR)/%.asm,$(OBJDIR)/%.o,$(ASM_FILES))


all: $(OBJDIR)/hypervisor.iso


$(OBJDIR)/%.o : $(SRC_DIR)/%.c
	$(C_COMPILER) $(C_COMPILER_FLAGS) $< -o $@

$(OBJDIR)/%.o : $(SRC_DIR)/%.asm
	$(ASSEMBLER) $(ASSEMBLER_FLAGS) $< -o $@

$(OBJDIR)/hypervisor.so: $(OBJ_FILES)
	$(LINKER) $(LINKER_FLAGS) $(OBJ_FILES) -o $@

$(OBJDIR)/hypervisor.iso: $(OBJDIR)/hypervisor.so $(ISO_DIR)/boot/grub/grub.cfg
	cp $< $(ISO_DIR)/boot/$(notdir $<)
	grub-mkrescue -o $@ $(ISO_DIR)

clean:
	rm $(OBJ_FILES)
	rm $(OBJDIR)/hypervisor.so
	rm $(OBJDIR)/hypervisor.iso
	rm $(ISO_DIR)/boot/hypervisor.so
	rm $(LOG_FILE)