ASM = nasm
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -I include -I ../libc/include
SRC_DIR ?= src
BUILD_DIR ?= build
LIBC_DIR ?= ../libc

OBJECTS = \
$(BUILD_DIR)/boot.o \
$(BUILD_DIR)/cpu/gdt.o \
$(BUILD_DIR)/cpu/gdt_flush.o \
$(BUILD_DIR)/cpu/idt.o \
$(BUILD_DIR)/cpu/idt_flush.o \
$(BUILD_DIR)/cpu/irq.o \
$(BUILD_DIR)/cpu/port.o \
$(BUILD_DIR)/cpu/paging.o \
$(BUILD_DIR)/cpu/pic.o \
$(BUILD_DIR)/cpu/syscall.o \
$(BUILD_DIR)/drivers/pata_pio.o \
$(BUILD_DIR)/drivers/tty.o \
$(BUILD_DIR)/drivers/keyboard.o \
$(BUILD_DIR)/drivers/timer.o \
$(BUILD_DIR)/drivers/cmos.o \
$(BUILD_DIR)/mm/pmm.o \
$(BUILD_DIR)/mm/vmm.o \
$(BUILD_DIR)/fs/fat32.o \
$(BUILD_DIR)/process.o \
$(BUILD_DIR)/utils/kprintf.o \
$(BUILD_DIR)/utils/kmalloc.o \
$(BUILD_DIR)/utils/kpanic.o \
$(BUILD_DIR)/utils/config.o \
$(BUILD_DIR)/kernel.o \

LIBC_OBJECTS = \
$(LIBC_DIR)/build/ctype.o \
$(LIBC_DIR)/build/string.o \
$(LIBC_DIR)/build/memory.o \

PHONY: all clean always

all: clean always $(BUILD_DIR)/minios.bin

$(BUILD_DIR)/minios.bin: $(OBJECTS)
	make -C $(LIBC_DIR)

	$(LD) -m elf_i386 -T linker.ld -o $@ $(OBJECTS) $(LIBC_OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	$(ASM) -f elf32 $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

always:
	mkdir -p $(BUILD_DIR)/kernel
	mkdir -p $(BUILD_DIR)/cpu
	mkdir -p $(BUILD_DIR)/drivers
	mkdir -p $(BUILD_DIR)/fs
	mkdir -p $(BUILD_DIR)/mm
	mkdir -p $(BUILD_DIR)/utils

clean:
	rm -f -r $(BUILD_DIR)