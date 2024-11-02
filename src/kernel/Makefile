ASM?=nasm
ASMFLAGS?=-f obj
CC16?=/opt/watcom/binl/wcc
CFLAGS16?=-i=include -4 -d3 -s -wx -ms -zl -zq -za99
LD16?=/opt/watcom/binl/wlink

BUILD_DIR?=build

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm) $(wildcard drivers/*.asm)

OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.obj, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.obj, $(SOURCES_ASM))

.PHONY: all clean

all: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LD16) NAME $(BUILD_DIR)/kernel.bin FILE \{ $(OBJECTS_ASM) $(OBJECTS_C) \} OPTION MAP=$(BUILD_DIR)/kernel.map @linker.ld

$(BUILD_DIR)/kernel/c/%.obj: %.c always
	$(CC16) $(CFLAGS16) -fo=$@ $<

$(BUILD_DIR)/kernel/asm/%.obj: %.asm always
	$(ASM) $(ASMFLAGS) $< -o $@

always:
	mkdir -p $(BUILD_DIR)/kernel/c
	mkdir -p $(BUILD_DIR)/kernel/asm
	mkdir -p $(BUILD_DIR)/kernel/asm/drivers

clean:
	rm -f $(BUILD_DIR)/kernel.bin