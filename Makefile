ASM=nasm
CC=gcc
LD=ld
CFLAGS=-m32 -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -I src/include
SRC_DIR?=src
BUILD_DIR?=build

OBJECTS= \
$(BUILD_DIR)/boot/boot.o \
$(BUILD_DIR)/kernel/cpu/gdt.o \
$(BUILD_DIR)/kernel/cpu/gdt_flush.o \
$(BUILD_DIR)/kernel/cpu/idt.o \
$(BUILD_DIR)/kernel/cpu/idt_flush.o \
$(BUILD_DIR)/kernel/cpu/irq.o \
$(BUILD_DIR)/kernel/drivers/port.o \
$(BUILD_DIR)/kernel/drivers/tty.o \
$(BUILD_DIR)/kernel/drivers/keyboard.o \
$(BUILD_DIR)/kernel/kernel.o \
$(BUILD_DIR)/kernel/console.o \
$(BUILD_DIR)/kernel/pic.o \
$(BUILD_DIR)/kernel/panic.o \
$(BUILD_DIR)/kernel/syscalls.o \
$(BUILD_DIR)/libc/string.o \
$(BUILD_DIR)/libc/memory.o \

PHONY: all clean always

all: clean always minios.iso

minios.iso: $(BUILD_DIR)/minios.iso

$(BUILD_DIR)/minios.iso: minios.bin
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub
	cp $(BUILD_DIR)/minios.bin isodir/boot/minios.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/minios.iso isodir

minios.bin: $(BUILD_DIR)/minios.bin

$(BUILD_DIR)/minios.bin: $(OBJECTS)
	$(LD) -m elf_i386 -T linker.ld -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	$(ASM) -f elf32 $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

always:
	mkdir -p $(BUILD_DIR)/boot
	mkdir -p $(BUILD_DIR)/kernel
	mkdir -p $(BUILD_DIR)/kernel/cpu
	mkdir -p $(BUILD_DIR)/kernel/drivers
	mkdir -p $(BUILD_DIR)/libc

clean:
	rm -f -R $(BUILD_DIR)/*
	rm -f -R isodir