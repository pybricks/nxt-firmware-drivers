.SUFFIXES:

default: all

include environment.mk
include targetdef.mk

C_OPTIMISATION_FLAGS = -Os
#C_OPTIMISATION_FLAGS = -Os -Xassembler -aslh
#C_OPTIMISATION_FLAGS = -O0

SVN_REV := $(shell svnversion -c ../.. | grep -E -o "[0-9]+" | tail -n 1)

CFLAGS = $(BASE_ABI_FLAGS) -mthumb \
	-ffreestanding -fsigned-char \
	$(C_OPTIMISATION_FLAGS) -g \
	-Wall -Winline -Werror-implicit-function-declaration \
	-I. -I$(VM_DIR) \
	-ffunction-sections -fdata-sections

LDFLAGS = $(LIB_ABI_FLAGS) -nostdlib -nodefaultlibs -Wl,-cref,--gc-sections

ALL_BIN := $(TARGETS)
ALL_OBJECTS := $(C_OBJECTS) $(S_OBJECTS)
ALL_ELF := $(ALL_BIN:.bin=.elf)
ALL_MAP := $(ALL_BIN:.bin=.map)
ALL_ASM := $(ALL_BIN:.bin=.asm) $(ALL_OBJECTS:.o=.asm)

.SECONDARY: $(ALL_ELF) $(ALL_MAP) $(ALL_OBJECTS)

.PHONY: all
all:  BuildMessage $(TARGET)

.PHONY: everything
everything: BuildMessage $(ALL_BIN) $(ALL_ASM)

.PHONY: TargetMessage
TargetMessage:
	@echo ""
	@echo "Building: $(TARGET)"
	@echo ""
	@echo "C objects: $(C_OBJECTS)"
	@echo ""
	@echo "Assembler objects: $(S_OBJECTS)"
	@echo ""
	@echo "LD source: $(LDSCRIPT_SOURCE)"
	@echo ""

.PHONY: BuildMessage
BuildMessage: TargetMessage EnvironmentMessage

.PHONY: clean
clean:  
	@echo "Removing All Objects"
	@rm -f $(ALL_OBJECTS)
	@echo "Removing target"
	@rm -f $(ALL_ELF) $(ALL_BIN)
	@echo "Removing map files"
	@rm -f $(ALL_MAP)
	@echo "Removing asm files"
	@rm -f $(ALL_ASM)

%.elf: %.lds $(C_OBJECTS) $(S_OBJECTS)
	@echo "### Linking $@ using linker script $<"
	$(CC) $(LDFLAGS) -Wl,-T,$<,-Map,${@:.elf=.map} -o $@ $(C_OBJECTS) $(S_OBJECTS) -lm -lc -lgcc

%.map: %.elf
	@true

%.bin: %.elf
	@echo "### Generating binary file $@ from $<"
	$(OBJCOPY) -O binary $< $@

# default rules for compiling sources

%.o: %.S
	@echo "### Assembling $< to $@"
	$(CC) $(CFLAGS) -c -o $@ $< 

%.o: %.c
	@echo "### Compiling $< to $@"
	$(CC) $(CFLAGS) -c -o $@ $< 


### rules for disassembling

%.asm: %.elf
	@echo "### Disassembling $< to $@"
	$(OBJDUMP) -z -x -d $< >$@

%.asm: %.o
	@echo "### Disassembling $< to $@"
	$(OBJDUMP) -z -x -d $< >$@

